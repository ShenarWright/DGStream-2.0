#pragma once

#define local

#ifdef local
	#define consumet "https://api.consumet.org"
	#define gogourl "https://api.consumet.org/anime/gogoanime"
#else
	#define consumet "https://api.consumet.org"
	#define gogourl "https://api.consumet.org/anime/gogoanime"
#endif
#define gogo "/anime/gogoanime"
#define topairing "/top-airing"
#define info "/info/"
#define watch "/watch/"
#define server "?server="
#define servers "/servers/"
#define gogocdn "gogocdn"
//#define search "/search"