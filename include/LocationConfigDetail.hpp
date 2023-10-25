#ifndef LOCATIONCONFIGDETAIL
#define LOCATIONCONFIGDETAIL

#include <string>

class LocationConfigDetail
{
	public:
		LocationConfigDetail(void);
		~LocationConfigDetail(void);

		void	setLocation(std::string const & info);
		std::string const & getVal(std::string const & key);

	private:
		LocationConfigDetail(LocationConfigDetail const & r);
		LocationConfigDetail &operator=(LocationConfigDetail const & r);
};

#endif
