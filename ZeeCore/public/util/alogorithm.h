#pragma once
namespace zee {
namespace algo {

	template<typename ContainerT, typename IterT, typename Pred>
	void remove_single(ContainerT& container, IterT first, IterT last, Pred pred) {
		for (; first != last; ++first) {
			if (pred(*first)) {
				container.erase(first);
				return;
			} 
		}
	}

	template<typename ContainerT, typename IterT, typename Pred>
	void remove_all(ContainerT& container, IterT first, IterT last, Pred pred) {
		for (; first != last; ) {
			if (pred(*first)) {
				first = container.erase(first);
			} else {
				++first;
			}
		}
	}

}//namespace algo
}//namespace zee
