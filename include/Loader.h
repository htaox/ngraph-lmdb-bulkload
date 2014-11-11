#ifndef LOADER_H
#define LOADER_H

#include <node.h>
#include <string>

class Loader : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit Loader(double value = 0);
  ~Loader();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static int OpenFile(std::string path, std::stringstream ss);
  static v8::Handle<v8::Value> Bulkload(const v8::Arguments& args);
  static v8::Persistent<v8::Function> constructor;
  double value_;
};

#endif