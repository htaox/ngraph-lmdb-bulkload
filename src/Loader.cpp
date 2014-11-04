#include <node.h>
#include "Loader.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace v8;

Persistent<Function> Loader::constructor;

Loader::Loader(std::string value) : value_(value){
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

/*
v8::String vs std::string
=========================

v8::String::Utf8Value param1(args[0]->ToString());
std::string from = std::string(*param1);
and if your just trying to convert a std::string to a v8::String then do

std::string something("hello world"); 
Handle<Value> something_else = String::New( something.c_str() );

Reference:
http://code.tutsplus.com/tutorials/writing-nodejs-addons--cms-21771
*/
Handle<Value> Loader::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Loader(...)`
    v8::String::Utf8Value str(args[0]->ToString());
	std::string s(*str);
	
	Loader* obj = new Loader(s);
    obj->Wrap(args.This());
    return args.This();
  } else {
    // Invoked as plain function `Loader(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
	return scope.Close(constructor->NewInstance(argc, argv));
  }
}

int Loader::OpenFile(std::string path) {
  std::stringstream ss;
  std::ifstream ifs;
  char* buffer;
  std::string temp;
  int length = 0;
  
  try{
		ifs.open (path.c_str(), std::ios::binary );
		if (!ifs) {
			retval_.assign(path + " does not exist");
			return 1;
		}
		// get length of file:
		ifs.seekg (0, std::ios::end);
		length = ifs.tellg();
		ifs.seekg (0, std::ios::beg);  
		// allocate memory:
		buffer = new (std::nothrow) char [length]; 
		if (buffer == 0){
		  retval_.assign(path + " is empty");
		  return 1;
		}
		ifs.read(buffer,length);  
		ifs.close();  
		ss << buffer;  
		delete[] buffer;
  }
  catch (std::exception& e){
		retval_.assign(path+" error caught-> "+e.what());
		return 1;
  }
  retval_.assign(path+" size-> "+std::to_string(length));
  return 0;
}

Handle<Value> Loader::Bulkload(const Arguments& args) {
  HandleScope scope;

  Loader* obj = ObjectWrap::Unwrap<Loader>(args.This());
  
  obj->OpenFile(obj->value_);

  return scope.Close(v8::String::New(obj->retval_.c_str()));
}

