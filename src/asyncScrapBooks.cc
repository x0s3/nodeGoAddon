#include <nan.h>
#include <node.h>
#include <unistd.h>
#include "../scrapBooks.h"

using namespace v8;
using std::string;

class Book {
    friend void BookAsync(uv_work_t*);
    friend void BookAsyncComplete(uv_work_t*, int);

    public:
        Book() = default;
        ~Book() = default;
        uv_work_t request;
        Persistent<Function> callback;
        string bookTitle = "";

    private:
        string data = "";
};

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void BookAsync(uv_work_t *request) {
    Book* book = static_cast<Book*>(request->data);
    char* cstr = &book->bookTitle[0];
    char* result = GetBookTitle(cstr);
    string str(result);

    book->data = str;
    sleep(3); // simulate hard work task
}

void BookAsyncComplete(uv_work_t *request, int status) {
    Nan::HandleScope scope;
    Book* book = static_cast<Book*>(request->data);

    Local<Value> argv[] = { 
        Nan::New<String>(book->data).ToLocalChecked() 
    };
    Local<Function> callback = Nan::New<Function>(book->callback);

    Nan::Call(callback, Nan::New<Object>(), 1, argv);

    delete book;
}

NAN_METHOD(MethodAsync) {
    Isolate* isolate = info.GetIsolate();
    Book* book = new Book();

    Nan::Utf8String utf8_value(info[0]);

    int len = utf8_value.length();
    string string_copy(*utf8_value, len);

    book->bookTitle = string_copy;
    book->request.data = book;

    Local<Function> callback = Local<Function>::Cast(info[1]);
    book->callback.Reset(isolate, callback);

    uv_queue_work(
        uv_default_loop(), 
        &book->request, 
        BookAsync, 
        BookAsyncComplete
    );

    info.GetReturnValue().Set(Nan::Undefined());
}

NAN_MODULE_INIT(init) {
    Nan::Set(
        target, 
        Nan::New("getAsyncBookTitle").ToLocalChecked(), 
        Nan::GetFunction(
            Nan::New<FunctionTemplate>(MethodAsync)
        ).ToLocalChecked()
    );
}

NODE_MODULE(async, init);