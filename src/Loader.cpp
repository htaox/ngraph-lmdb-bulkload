#include <node.h>
#include "Loader.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace v8;

Persistent<Function> Loader::constructor;

Loader::Loader(double value) : value_(value) {
}

Loader::~Loader() {
}

void Loader::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Loader"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("bulkLoad"),
      FunctionTemplate::New(Bulkload)->GetFunction());
  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Loader"), constructor);
}

Handle<Value> Loader::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Loader(...)`
    double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    Loader* obj = new Loader(value);
    obj->Wrap(args.This());
    return args.This();
  } else {
    // Invoked as plain function `Loader(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    return scope.Close(constructor->NewInstance(argc, argv));
  }
}

int Loader::OpenFile(std::string path, std::stringstream ss) {
  std::ifstream ifs;
  char* buffer;
  std::string temp;
  int length = 0;
  
  try{
		ifs.open (path.c_str(), std::ios::binary );
		if (!ifs) return 1;
		// get length of file:
		ifs.seekg (0, std::ios::end);
		length = ifs.tellg();
		ifs.seekg (0, std::ios::beg);  
		// allocate memory:
		buffer = new (std::nothrow) char [length]; 
		if (buffer == 0){
		  //get_time(st, timestamp);
		  //out_l << timestamp << "|Error thrown by bad allocation attempt.  Buffer requested: " << length << "\n"; 
		  //out_l.flush();
		  return 1;
		}
		ifs.read(buffer,length);  
		ifs.close();  
		ss << buffer;  
		delete[] buffer;
  }
  catch (std::exception& e){
		//get_time(st, timestamp);
		//out_l << timestamp << "|Error-> " << e.what() << "\n";
		//out_l.flush();
  }
  return length;
}

Handle<Value> Loader::Bulkload(const Arguments& args) {
  HandleScope scope;

  Loader* obj = ObjectWrap::Unwrap<Loader>(args.This());
  //obj->value_ += 1;
  std::string s;
  std::stringstream ss;

  return scope.Close(Number::New(obj->value_));
}

