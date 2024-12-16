
template <typename T>
class CompactList {
	static const unsigned int	max_size = 128;
	T							fds[max_size];
	int							size = 0;

public:
	T&	at(int k) {
		return (fds[k]);
	}

	bool	add(const T& pfd) {
		if (size >= max_size)
			return (false);
		fds[size++] = pfd;
		return (true);
	}

	int	compact() {
		int rcount = 0;

		for (int k = 0; k < size; k++) {
			if (fds[k].fd == -1)
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