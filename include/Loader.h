#ifndef LOADER_H
#define LOADER_H

#include <node.h>
#include <string>

class Loader : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit Loader(std::string value);
  ~Loader();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Bulkload(const v8::Arguments& args);
  static v8::Handle<v8::Value> Stats(const v8::Arguments& args);
  static v8::Persistent<v8::Function> constructor;
  int OpenFile(std::string path);
  int Test(std::string path);
  int GetStats();
  std::string value_;
  std::string retval_;
};

#endif