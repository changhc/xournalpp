#include "util/OutputStream.h"

#include <glib.h>

#include "util/GzUtil.h"
#include "util/i18n.h"

OutputStream::OutputStream() = default;

OutputStream::~OutputStream() = default;

void OutputStream::write(const std::string& str) { write(str.c_str(), str.length()); }

void OutputStream::write(const char* str) { write(str, strlen(str)); }

////////////////////////////////////////////////////////
/// GzOutputStream /////////////////////////////////////
////////////////////////////////////////////////////////

GzOutputStream::GzOutputStream(fs::path file): file(std::move(file)) {
    this->fp = GzUtil::openPath(this->file, "w");
    if (this->fp == nullptr) {
        this->error = FS(_F("Error opening file: \"{1}\"") % this->file.u8string());
    }
}

GzOutputStream::~GzOutputStream() {
    if (this->fp) {
        close();
    }
    this->fp = nullptr;
}

auto GzOutputStream::getLastError() -> std::string& { return this->error; }

void GzOutputStream::write(const char* data, int len) { gzwrite(this->fp, data, len); }

void GzOutputStream::close() {
    if (this->fp) {
        gzclose(this->fp);
        this->fp = nullptr;
    }
}
