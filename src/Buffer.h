#pragma once

struct BufferDescriptor
{
   
};

class Buffer
{
public:
    Buffer();
    virtual ~Buffer();

    inline long GetBindFlags() const
    {
        return bindFlags_;
    }

private:
    long bindFlags_ = 0;
};
