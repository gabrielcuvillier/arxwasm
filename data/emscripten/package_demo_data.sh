#!/usr/bin/env bash
python $EMSCRIPTEN/tools/file_packager.py data.pak --preload $1/data.pak@/data_demo/arx/   --js-output=data00.js --use-preload-cache --no-heap-copy

python $EMSCRIPTEN/tools/file_packager.py data2.pak --preload $1/data2.pak@/data_demo/arx/ --js-output=data01.js --use-preload-cache --no-heap-copy

python $EMSCRIPTEN/tools/file_packager.py loc.pak --preload $1/loc.pak@/data_demo/arx/   --js-output=data02.js --use-preload-cache --no-heap-copy

python $EMSCRIPTEN/tools/file_packager.py sfx.pak --preload $1/sfx.pak@/data_demo/arx/   --js-output=data03.js --use-preload-cache --no-heap-copy

python $EMSCRIPTEN/tools/file_packager.py speech.pak --preload $1/speech.pak@/data_demo/arx/   --js-output=data04.js --use-preload-cache --no-heap-copy
