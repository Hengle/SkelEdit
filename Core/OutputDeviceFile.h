
// Will automatically create directory for file
class COutputDeviceFile : public COutputDevice
{
protected:
	FILE	*log;
public:
	COutputDeviceFile()
	:	log(NULL)
	{}
	COutputDeviceFile(const char *fileName, bool erasePrevious = false)
	{
		Open(fileName, erasePrevious);
	}
	~COutputDeviceFile()
	{
		Unregister();
		Close();
	}
	virtual void Write(const char *str)
	{
		if (!log || !str[0]) return;
		// remove color codes
		int len = strlen(str);
		if (!len) return;
		// write to file
		if (fwrite(str, 1, len, log) < len)
		{
			appNotify("Unable to write to log");
			fclose(log);
			log = NULL;
		}
	}
	virtual void Flush()
	{
		if (log) fflush(log);
	}
	void Open(const char *fileName, bool erasePrevious = false)
	{
//		appMakeDirectoryForFile(fileName);
		log = fopen(fileName, erasePrevious ? "w" : "a");
		if (!log)
			appNotify("Cannot write to file \"%s\"", fileName);
	}
	void Close()
	{
		if (log) fclose(log);
		log = NULL;
	}
	inline bool IsOpened() const
	{
		return log != NULL;
	}
};


class COutputDeviceStdout : public COutputDeviceFile
{
public:
	COutputDeviceStdout()
	{
		log = stdout;
	}
};
