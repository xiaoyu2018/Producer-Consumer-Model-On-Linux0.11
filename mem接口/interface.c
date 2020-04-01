void AddMemMap(unsigned long PhysicalAdd)
{
		unsigned long page_num = MAP_NR(PhysicalAdd);
		mem_map[page_num]++;
}


void unmap(const void *linearaddr)
{
    unsigned long *page_table;
    //获得线性地址对应的页目录索引，页目录项中包含指向页表的指针
    //线性地址最高10位（22位到31位）为页目录索引
    //右移20位后按位与111111111100，即保留前十位
    page_table = (unsigned long *)((linearaddr >> 20) & 0xffc);
    //页目录项最后一位（P）为页目录项有效标志位，当P=1时此项有效
    //有效则获取页目录项指向的地址，即页表的首地址
    if ((*page_table) & 1)
    {
        page_table = (unsigned long *)(0xfffff000 & *page_table);
        //将对应页表项置零以解除映射
        //线性地址中间10位（22位到31位）为页表索引
        //右移12位按位与001111111111，即保留中间10位
        page_table[linearaddr >> 12) & 0x3ff] = 0;
    }
}