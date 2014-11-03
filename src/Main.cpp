#include "main.h"
#include <node.h>
#include "Loader.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Loader::Init(exports);
}

NODE_MODULE(NodeAddon, InitAll)