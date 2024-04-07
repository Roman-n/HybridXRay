
#pragma once
#include "../xrEngine/xrLevel.h"
#pragma pack(push,1)

#ifdef AI_COMPILER
struct NodeCompressed6 {
public:
	u8				data[11];
private:
	
	ICF	void link(u8 link_index, u32 value)
	{
		value			&= 0x001fffff;
		switch (link_index) {
			case 0 : {
				value	|= (*(u32*)data) & 0xffe00000;
				CopyMemory(data, &value, sizeof(u32));
				break;
			}
			case 1 : {
				value	<<= 5;
				value	|= (*(u32*)(data + 2)) & 0xfc00001f;
				CopyMemory(data + 2, &value, sizeof(u32));
				break;
			}
			case 2 : {
				value	<<= 2;
				value	|= (*(u32*)(data + 5)) & 0xff800003;
				CopyMemory(data + 5, &value, sizeof(u32));
				break;
			}
			case 3 : {
				value	<<= 7;
				value	|= (*(u32*)(data + 7)) & 0xf000007f;
				CopyMemory(data + 7, &value, sizeof(u32));
				break;
			}
		}
	}
	
	ICF	void light(u8 value)
	{
		data[10]		|= value << 4;
	}

public:
	u16				cover0 : 4;
	u16				cover1 : 4;
	u16				cover2 : 4;
	u16				cover3 : 4;
	u16				plane;
	NodePosition	p;

	ICF	u32	link(u8 index) const
	{
		switch (index) {
			case 0 :	return	((*(u32*)data) & 0x001fffff);
			case 1 :	return	(((*(u32*)(data + 2)) >> 5) & 0x001fffff);
			case 2 :	return	(((*(u32*)(data + 5)) >> 2) & 0x001fffff);
			case 3 :	return	(((*(u32*)(data + 7)) >> 7) & 0x001fffff);
			default :	NODEFAULT;
		}
#ifdef DEBUG
		return			(0);
#endif
	}
	
	ICF	u8	light() const
	{
		return			(data[10] >> 4);
	}
	
	ICF	u16	cover(u8 index) const
	{
		switch (index) {
			case 0 : return(cover0);
			case 1 : return(cover1);
			case 2 : return(cover2);
			case 3 : return(cover3);
			default : NODEFAULT;
		}
#ifdef DEBUG
		return				(u8(-1));
#endif
	}

	friend class ILevelGraph;
	friend struct CNodeCompressed;
	friend class CNodeRenumberer;
};									// 2+5+2+11 = 20b
#endif

#pragma pack	(pop)

