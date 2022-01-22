#pragma once

#include <deque>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

class InputStream
{
public:
    virtual uint8_t read_8() = 0;
    virtual uint8_t peek_8() = 0;

    uint16_t read_16();
    uint32_t read_32();

    virtual long size() = 0;
};

class OutputStream
{
public:
    virtual void write_8(uint8_t value) = 0;

    void write_16(uint16_t value);
    void write_32(uint32_t value);
};

class BufferStream : public InputStream, public OutputStream
{
private:
    std::deque<uint8_t> queue;

public:
    void write_8(uint8_t value) override;

    uint8_t read_8() override;
    uint8_t peek_8() override { return queue.front(); }

    void clear() { queue.clear(); }
    std::string as_string() const;

    long size() override { return queue.size(); }
};

class InputFileStream : public InputStream
{
private:
    std::ifstream input;

public:
    InputFileStream(const std::string& filename);

    void close() { input.close(); }
    bool is_open() const { return input.is_open(); }

    uint8_t read_8() override { return input.get(); }
    uint8_t peek_8() override { return input.peek(); }

    long size() override;
};

class OutputFileStream : public OutputStream
{
private:
    std::ofstream output;

public:
    OutputFileStream(const std::string& filename);

    void close() { output.close(); }
    bool is_open() const { return output.is_open(); }

    void write_8(uint8_t value) override;
};
