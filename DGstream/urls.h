#pragma once

//#define local

#ifdef local
	#define consumet "https://c.delusionz.xyz"
	#define gogourl "https://c.delusionz.xyz/anime/gogoanime"
#else
	#define consumet "http://192.168.137.187:3000"
	#define gogourl "http://192.168.137.187:3000/anime/gogoanime"
#endif
#define gogo "/anime/gogoanime"
#define topairing "/top-airing"
#define recent "/recent-episodes?type=2"
#define info "/info/"
#define watch "/watch/"
#define server "?server="
#define servers "/servers/"
#define gogocdn "gogocdn"
//#define search "/search"