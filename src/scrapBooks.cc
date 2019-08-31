#include <node.h>
#include <unistd.h>
#include "../scrapBooks.h"

namespace scrapBooks {

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Local;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::String;
using v8::Value;

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Local<Context> context = isolate->GetCurrentContext();
  Local<Function> cb = Local<Function>::Cast(args[1]);

  const unsigned argc = 1;

  String::Utf8Value str(isolate,args[0]);
  const char* cstr = ToCString(str);
  char* charstr = const_cast<char*>(cstr);
  char* result = GetBookTitle(charstr);

  Local<Value> argv[argc] = {
      String::NewFromUtf8(
        isolate,
        result,
        NewStringType::kNormal).ToLocalChecked() 
  };

  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "getSyncBookTitle", Method);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace scrapBooks
