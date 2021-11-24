#pragma once
namespace zee {
namespace algo {

	template<typename ContainerT, typename IterT, typename Pred>
	bool remove_single(ContainerT& container, IterT first, IterT last, const Pred& pred) {
		for (; first != last; ++first) {
			if (pred(*first)) {
				container.erase(first);
				return true;
			} 
		}
		return false;
	}

	template<typename ContainerT, typename IterT, typename Pred>
	bool remove_all(ContainerT& container, IterT first, IterT last, const Pred& pred) {
		bool ret = false;
		for (; first != last; ) {
			if (pred(*first)) {
				first = container.erase(first);
				ret = true;
			} else {
				++first;
			}
		}
		return ret;
	}

}//namespace algo
}//namespace zee
