#ifndef RONZHU_QUOTECIRCULARBUFFER_H
#define RONZHU_QUOTECIRCULARBUFFER_H

#include <./boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>

template<typename T>
class QuoteCircularBuffer
{
public:
	QuoteCircularBuffer(int item_size) :buffer_(item_size)
	{
	};

	~QuoteCircularBuffer()
	{
	};

	bool push_back(const T& item)
	{
		boost::unique_lock<boost::shared_mutex> write_lock(buffer_mutex_);
		if (0 == buffer_.capacity()) { return false; }

		if (!buffer_.full()) {
			buffer_.push_back(item);
			return true;
		}
		return false;
	};

	bool push_front(const T& item)
	{
		boost::unique_lock<boost::shared_mutex> write_lock(buffer_mutex_);
		if (0 == buffer_.capacity()) { return false; }

		if (!buffer_.full()) {
			buffer_.push_front(item);
			return true;
		}
	};

	void clear()
	{
		boost::unique_lock<boost::shared_mutex> write_lock(buffer_mutex_);
		buffer_.clear();
	};

	T& front()
	{
		boost::shared_lock<boost::shared_mutex> read_lock(buffer_mutex_);
		return buffer_.front();
	};

	void pop_front()
	{
		boost::unique_lock<boost::shared_mutex> write_lock(buffer_mutex_);
		buffer_.pop_front();
	};

	void pop_back()
	{
		boost::unique_lock<boost::shared_mutex> write_lock(buffer_mutex_);
		buffer_.pop_back();
	};

	void rset_capacity(size_t num)
	{
		boost::shared_lock<boost::shared_mutex> read_lock(buffer_mutex_);
		//buffer_.resize(num);
		buffer_.rset_capacity(num);
	}

	unsigned int capacity()
	{
		boost::shared_lock<boost::shared_mutex> read_lock(buffer_mutex_);
		return buffer_.capacity();
	};

	unsigned int size()
	{
		boost::shared_lock<boost::shared_mutex> read_lock(buffer_mutex_);
		return buffer_.size();
	};

	bool empty()
	{
		boost::shared_lock<boost::shared_mutex> read_lock(buffer_mutex_);
		return buffer_.empty();
	};

	bool full()
	{
		boost::shared_lock<boost::shared_mutex> read_lock(buffer_mutex_);
		return buffer_.full();
	};

	T& at(size_t index)
	{
		boost::shared_lock<boost::shared_mutex> read_lock(buffer_mutex_);
		try {
			buffer_.at(index);
		}
		catch (std::out_of_range& ex) {
			throw(ex);
		}

		return buffer_[index];
	};

private:
	boost::circular_buffer<T> buffer_;
	boost::shared_mutex buffer_mutex_;
	boost::mutex mutex_;
};

#endif // RONZHU_PUBLICMODULE_H
