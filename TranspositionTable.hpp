#include <vector>
#include <cstring>

class TranspositionTable {
	private:
		struct Entry {
			uint64_t key: 56;
			uint8_t val;
		};

		std::vector<Entry> T;

		unsigned int index(uint64_t key) const {
			return key % T.size();
		}

	public:
		TranspositionTable(unsigned int size): T(size) {}

		void reset() {
			memset(&T[0], 0, T.size() * sizeof(Entry));
		}

		void put(uint64_t key, uint8_t val) {
			unsigned int i = index(key);
			T[i].key = key;
			T[i].val = val;
		}

		uint8_t get(uint64_t key) const {
			unsigned int i = index(key);
			if (T[i].key == key) {
				return T[i].val;
			}
			else {
				return 0;
			}
		}
};
