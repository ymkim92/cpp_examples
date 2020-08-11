#include<bits/stdc++.h>

using namespace std;
//Implement the class Box  
//l,b,h are integers representing the dimensions of the box

// The class should have the following functions : 

// Constructors: 
// Box();
// Box(int,int,int);
// Box(Box);


class Box
{
private:
    int length, breadth, height;
public:
    Box()
    {
        length = breadth = height = 0;
    }
    Box(int l, int b, int h)
    {
        length = l;
        breadth = b;
        height = h;
    }
    Box(Box &b)
    {
        length = b.length;
        breadth = b.breadth;
        height = b.height;
    }
    int getLength() { return length; }
    int getBreadth() { return breadth; }
    int getHeight() { return height; }
    long long CalculateVolume() 
    {
        long long volume;
        volume = static_cast<long long>(length) * breadth * height;
        return volume;
    }

    friend bool operator<(const Box& b1, const Box& b2);
    friend ostream& operator<<(ostream &out, const Box& b);
};

bool operator<(const Box& b1, const Box& b2)
{
    if (b1.length < b2.length)
        return true;
    if ( (b1.length == b2.length) && (b1.breadth < b2.breadth) )
        return true;
    if ( (b1.length == b2.length) && (b1.breadth == b2.breadth) && (b1.height < b2.height) )
        return true;
    
    return false;
}

ostream& operator<<(ostream &out, const Box&b)
{
    out << b.length << " " << b.breadth << " " << b.height;
    return out;
}



void check2()
{
	int n;
	cin>>n;
	Box temp;
	for(int i=0;i<n;i++)
	{
		int type;
		cin>>type;
		if(type ==1)
		{
			cout<<temp<<endl;
		}
		if(type == 2)
		{
			int l,b,h;
			cin>>l>>b>>h;
			Box NewBox(l,b,h);
			temp=NewBox;
			cout<<temp<<endl;
		}
		if(type==3)
		{
			int l,b,h;
			cin>>l>>b>>h;
			Box NewBox(l,b,h);
			if(NewBox<temp)
			{
				cout<<"Lesser\n";
			}
			else
			{
				cout<<"Greater\n";
			}
		}
		if(type==4)
		{
			cout<<temp.CalculateVolume()<<endl;
		}
		if(type==5)
		{
			Box NewBox(temp);
			cout<<NewBox<<endl;
		}

	}
}

int main()
{
	check2();
}