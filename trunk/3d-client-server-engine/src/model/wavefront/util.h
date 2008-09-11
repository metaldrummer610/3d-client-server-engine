#ifndef WAVEFRONT_UTIL_H
#define WAVEFRONT_UTIL_H


template <typename T,typename InputIterator>
T merge(InputIterator iter_beg_1,InputIterator iter_end_1)
{
	T t;
	for(;iter_beg_1 != iter_end_1 && 
		 iter_beg_2 != iter_end_2 &&
		 iter_beg_3 != iter_end_3; 
		 ++iter_beg_1,++iter_beg_2,++iter_beg_3)
	{
		t.push_back(*iter_beg_1);
		t.push_back(*iter_beg_2);
		t.push_back(*iter_beg_3);
	}
}

#endif//WAVEFRONT_UTIL_H
