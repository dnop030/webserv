#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

class configFile
{
	public:
		configFile(void);
		~configFile(void);

		void	readConfigFile(char const * fileName);

	private:
		configFile(configFile const & ref);
		configFile	&operator=(configFile const & ref);
};

#endif
