// Abstract class
class OdEntryBase
{
public:
	OdEntryBase()
	:m_index(0xFFFF)
    {}

	OdEntryBase(uint16_t a_index)
	:m_index(a_index)
    {}

	virtual ~OdEntryBase(){}
	virtual size_t GetSize() const = 0;
	virtual void InitSize() = 0;

private:
	uint16_t  m_index;
};

template <typename T>
class OdEntry: public OdEntryBase
{
public:
	OdEntry() : OdEntryBase()
	{}
	: OdEntryBase(a_index)
	, m_value(0)
	{}
	const T& GetValue() const { return m_value; }
	size_t GetSize() const
	{
		return sizeof(T);
	}
	OdEntry& operator =(const T& a_rhs)
	{
		m_value = a_rhs;
		return *this;
	}
	operator T() const
	{
		return m_value;
	}

private:
	T m_value;
};