
template <typename T>
class CompactList {
	static const unsigned int	max_size = 128;
	T							fds[max_size];
	unsigned int				size;

public:

	CompactList() :
		size(0)
	{}

	T&	at(int k) {
		return (fds[k]);
	}

	bool	add(const T& pfd) {
		if (size >= max_size)
			return (false);
		fds[size++] = pfd;
		return (true);
	}

	int	compact(bool (*predicat)(const T&)) {
		int rcount = 0;
		
		for (unsigned int k = 0; k < size; k++) {
			if (predicat(fds[k]))
				rcount++;
			else
				fds[k - rcount] = fds[k];
		}
		size -= rcount;
		return (rcount);
	}

	int	get_size() const {
		return (size);
	}

	T	*get_data() {
		return (fds);
	}
};