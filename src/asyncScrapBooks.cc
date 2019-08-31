#include <nan.h>
#include <node.h>
#include <unistd.h>
#include "../scrapBooks.h"

using namespace v8;
using std::string;

class Work {
    friend void WorkAsync (uv_work_t*);
    friend void WorkAsyncComplete (uv_work_t*, int);

    public:
        Work() = default;
        ~Work() = default;
        uv_work_t request;
        Persistent<Function> callback;
        string bookTitle = "";

    private:
        string data = "";
};

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void WorkAsync (uv_work_t *request) {
    Work* work = static_cast<Work*>(request->data);
    char* cstr = &work->bookTitle[0];
    char* result = GetBookTitle(cstr);
    std::string str(result);

    work->data = str;
    sleep(3); // simulate hard work task
}

void WorkAsyncComplete (uv_work_t *request, int status) {
    Nan::HandleScope scope;
    Work* work = static_cast<Work*>(request->data);

    Local<Value> argv[] = { 
        Nan::New<String>(work->data).ToLocalChecked() 
    };
    Local<Function> callback = Nan::New<Function>(work->callback);

    Nan::Call(callback, Nan::New<Object>(), 1, argv);

    delete work;
}

NAN_METHOD (MethodAsync) {
    Isolate* isolate = info.GetIsolate();
    Work* work = new Work();

    Nan::Utf8String utf8_value(info[0]);

    int len = utf8_value.length();
    std::string string_copy(*utf8_value, len);

    work->bookTitle = string_copy;
    work->request.data = work;

    Local<Function> callback = Local<Function>::Cast(info[1]);
    work->callback.Reset(isolate, callback);

    uv_queue_work(
        uv_default_loop(), 
        &work->request, 
        WorkAsync, 
        WorkAsyncComplete
    );

    info.GetReturnValue().Set(Nan::Undefined());
}

NAN_MODULE_INIT (init) {
    Nan::Set(
        target, 
        Nan::New("getAsyncBookTitle").ToLocalChecked(), 
        Nan::GetFunction(
            Nan::New<FunctionTemplate>(MethodAsync)
        ).ToLocalChecked()
    );
}

NODE_MODULE(async, init);