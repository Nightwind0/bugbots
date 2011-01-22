const int NUM_WORDS = 3;
const int MAX_FLAG = 32 * NUM_WORDS;

class Flag
{
private:
    long int flag[NUM_WORDS];
    long int FlagNumToBit(int FlagNum);
public:
    int SetFlag(int FlagNum);
    int RemoveFlag(int FlagNum);
    bool IsSet(int FlagNum);
    bool operator[](int FlagNum);
    int operator+=(int FlagNum);
    int operator-=(int FlagNum);
};


#if 0
int Flag::SetFlag(int FlagNum)
{
    int FlagVar, ReturnCode;
    long int FlagBit;
    
    if(FlagNum > MAX_FLAG)
	ReturnCode = -1;
    else
    {
	FlagVar = FlagNum / 32;
	FlagBit = FlagNumToBit(FlagNum % 32);
	ReturnCode = 0;
	
	flag[FlagVar] |= FlagBit;
    }
    
    return ReturnCode;
}

int Flag::RemoveFlag(int FlagNum)
{
    int FlagVar, ReturnCode;
    long int FlagBit;
    
    if(FlagNum > MAX_FLAG)
	ReturnCode = -1;
    else
    {
	FlagVar = FlagNum / 32;
	FlagBit = ~FlagNumToBit(FlagNum % 32);
	ReturnCode = 0;
	
	flag[FlagVar] &= FlagBit;
    }
    
    return ReturnCode;
}

bool Flag::IsSet(int FlagNum)
{
    int FlagVar;
    long int FlagBit;
    bool ReturnCode;
    
    if(FlagNum > MAX_FLAG)
	ReturnCode = false;
    else
    {
	FlagVar = FlagNum / 32;
	FlagBit = FlagNumToBit(FlagNum % 32);
	
	if(flag[FlagVar] & FlagBit)
	    ReturnCode = true;
	else
	    ReturnCode = false;
    }
    
    return ReturnCode;
}

long int Flag::FlagNumToBit(int FlagNum)
{
    return 1 << FlagNum;
}

bool Flag::operator[](int FlagNum)
{
    return IsSet(FlagNum);
}

int Flag::operator+=(int FlagNum)
{
    return SetFlag(FlagNum);
}

int Flag::operator-=(int FlagNum)
{
    return RemoveFlag(FlagNum);
}

#endif

