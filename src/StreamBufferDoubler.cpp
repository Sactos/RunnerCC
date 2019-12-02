#ifndef STREAM_BUFFER_DOUBLER_CPP
#define STREAM_BUFFER_DOUBLER_CPP

#include <fstream>
#include <vector>
#include <assert.h> 

class StreamBufferDoubler : public std::streambuf {
public:
    StreamBufferDoubler(std::streambuf* buf1, std::streambuf* buf2) :
            _buffer1(buf1), _buffer2(buf2), _buffer(128)
    {
        assert(_buffer1 && _buffer2);

        setg(0, 0, 0);
        setp(_buffer.data(), _buffer.data() + _buffer.size());
    }

    ~StreamBufferDoubler() {
        sync();
    }

    void imbue(const std::locale& loc) {
        _buffer1->pubimbue(loc);
        _buffer2->pubimbue(loc);
    }

    std::streampos seekpos(std::streampos sp, std::ios_base::openmode which) {
        return seekoff(sp, std::ios_base::cur, which);
    }

    std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which) {
        if (which | std::ios_base::in)
            throw(std::runtime_error("Can't use this class to read data"));
        //CUAL RETORNO?
        _buffer1->pubseekoff(off, way, which);
        return _buffer2->pubseekoff(off, way, which);
    }

    int overflow(int c) {
        int retValue = sync() ? EOF : 0;
        sputc(c);
        return retValue;
    }

    int sync() {
        _buffer1->sputn(pbase(), pptr() - pbase());
        _buffer2->sputn(pbase(), pptr() - pbase());
        setp(_buffer.data(), _buffer.data() + _buffer.size());
        return _buffer1->pubsync() | _buffer2->pubsync();
    }

private:
    std::streambuf* _buffer1;
    std::streambuf* _buffer2;
    std::vector<char> _buffer;
};

#endif