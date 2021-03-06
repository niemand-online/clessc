#include "less/css/SourceMapWriter.h"

using namespace std;

const char* SourceMapWriter::base64 =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

SourceMapWriter::SourceMapWriter(ostream& sourcemap,
                                 list<const char*>& sources,
                                 const char* out_filename,
                                 const char* rootpath,
                                 const char* basepath)
    : sourcemap_h(sourcemap), sources(sources) {
  lastDstColumn = 0;
  lastSrcFile = 0;
  lastSrcLine = 0;
  lastSrcColumn = 0;
  writePreamble(out_filename, rootpath, basepath);
}

void SourceMapWriter::writePreamble(const char* out_filename,
                                    const char* rootpath,
                                    const char* basepath) {
  list<const char*>::iterator it;
  const char* source;
  size_t bp_l = 0;

  if (basepath != nullptr)
    bp_l = strlen(basepath);

  sourcemap_h << "{";

  sourcemap_h << "\"version\" : 3,";

  if (rootpath != nullptr) {
    sourcemap_h << "\"sourceRoot\": \"" << rootpath << "\",";
  }

  sourcemap_h << "\"file\": ";

  if (basepath != nullptr && strncmp(out_filename, basepath, bp_l) == 0) {
    out_filename += bp_l;
  }

  sourcemap_h << "\"" << out_filename << "\",";

  sourcemap_h << "\"sources\": [";

  for (it = sources.begin(); it != sources.end(); it++) {
    if (it != sources.begin())
      sourcemap_h << ",";
    source = *it;

    if (basepath != nullptr && strncmp(source, basepath, bp_l) == 0) {
      source += bp_l;
    }
    sourcemap_h << "\"" << source << "\"";
  }

  sourcemap_h << "],";

  sourcemap_h << "\"names\": [],";
  sourcemap_h << "\"mappings\": \"";
}

void SourceMapWriter::close() {
  sourcemap_h << "\"}" << endl;
}

void SourceMapWriter::writeMapping(unsigned int column, const Token& source) {
  char buffer[10];
  size_t len = encodeMapping(column, source, buffer);

  sourcemap_h.write(buffer, len);
  sourcemap_h.write(",", 1);
}

void SourceMapWriter::writeNewline() {
  sourcemap_h.write(";", 1);
  lastDstColumn = 0;
}

size_t SourceMapWriter::sourceFileIndex(const char* file) {
  list<const char*>::iterator i;
  size_t pos = 0;

  for (i = sources.begin(); i != sources.end(); i++, pos++) {
    if (*i == file)
      return pos;
  }
  return 0;
}

size_t SourceMapWriter::encodeMapping(unsigned int column,
                                      const Token& source,
                                      char* buffer) {
  size_t srcFileIndex = sourceFileIndex(source.source);
  char* start = buffer;

  buffer += encodeField(static_cast<int>(column - lastDstColumn), buffer);
  buffer += encodeField(static_cast<int>(srcFileIndex - lastSrcFile), buffer);
  buffer += encodeField(static_cast<int>(source.line - lastSrcLine), buffer);
  buffer += encodeField(static_cast<int>(source.column - lastSrcColumn), buffer);

  lastDstColumn = column;
  lastSrcFile = srcFileIndex;
  lastSrcLine = source.line;
  lastSrcColumn = source.column;

  return buffer - start;
}

size_t SourceMapWriter::encodeField(int field, char* buffer) {
  // convert to unsigned to get the binary right
  int value = abs(field);
  int current = (value & 0xF) << 1;
  size_t pos = 0;

  if (field < 0) {
    current |= 1;
  }

  value = value >> 4;

  while (value != 0) {
    buffer[pos] = base64[current | (1 << 5)];
    current = value & 0x1F;

    pos++;
    value = value >> 5;
  }

  buffer[pos] = base64[current];
  return pos + 1;
}
