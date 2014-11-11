#include "main.h"
#include <node.h>
#include "Loader.h"

using namespace v8;

/**
	@usage
	var NgraphLmdb = require('./x64/Debug/NgraphLmdb');
	var loader = new NgraphLmdb.Loader('some/path/to/file');
	//Execute!
	loader.bulkload();
**/
void InitAll(Handle<Object> exports) {
  Loader::Init(exports);
}

NODE_MODULE(NgraphLmdb, InitAll)