#pragma once
#include <iostream>

class membuf : public std::basic_streambuf<char> {
public:
	membuf(const uint8_t *p, size_t l) : p(p), l(l) {
		setg((char*)p, (char*)p, (char*)p + l);
	}

	virtual pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) override
	{
		if (dir == std::ios_base::cur)
			gbump((int)off);
		else if (dir == std::ios_base::end)
			setg((char*)p, (char*)p + l + off, (char*)p + l);
		else if (dir == std::ios_base::beg)
			setg((char*)p, (char*)p + off, (char*)p + l);

		return gptr() - eback();
	}

	virtual pos_type seekpos(std::streampos pos, std::ios_base::openmode mode) override
	{
		return seekoff(pos - pos_type(off_type(0)), std::ios_base::beg, mode);
	}
	const uint8_t* p;
	size_t l;
};

class memstream : public std::istream {
public:
	memstream(const uint8_t *p, size_t l) :
		std::istream(&_buffer),
		_buffer(p, l) {
		rdbuf(&_buffer);
	}

	membuf _buffer;
};