#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>



class Log {
public:
    enum Level {
        INFO = 1 << 0,
        DEBUG = 1 << 1,
        WARNING = 1 << 2,
        ERROR = 1 << 3,
		FUNCTION = 1 << 4,
		ALL = ~0
    };

	static Log& Instance() {
		static Log	instance;
		return (instance);
	}

    static Log& out(Level level = INFO) {
		Log&  instance = Instance()(level);
		if (instance.lock & instance.key) {
			return (instance << "[" << LevelToString(level) << "-" << count++ << "\e[0m]");
		}
		return (instance);
    }
 
	static void AddLevel(Level level) {
		Log& instance = Instance();
		instance.lock = Level(instance.lock | level);
	}

	static void RemoveLevel(Level level) {
		Log& instance = Instance();
		instance.lock = Level(instance.lock & ~level);
	}

    static void SetLevel(Level level) {
		
		Instance().lock = level;
    }

    static void SetOutput(std::ostream* output_stream) {
		Log& instance = Instance();
        std::lock_guard<std::mutex> lg(instance.mutex_);
        instance.output = output_stream;
    }

    template<typename T>
    Log& operator<<(const T& msg) {
        std::lock_guard<std::mutex> lg(mutex_);
        if ((lock & key) && output) {
            *output << msg;
        }
        return (*this);
    }

    Log& operator()(Level level) {
        std::lock_guard<std::mutex> lg(mutex_);
        key = level;
        return (*this);
    }

private:
	
    // Constructeur privé pour le singleton
    Log() : output(&std::cout), key(INFO), lock(INFO) {}

    static std::string LevelToString(Level level) {
        switch (level) {
            case INFO: return "\e[0;37mI";
            case DEBUG: return "\e[0;32mD";
            case WARNING: return "\e[0;33mW";
            case ERROR: return "\e[0;31mE";
			case FUNCTION: return "\e[0;36mF";
            default: return " ";
        }
    }

    std::ostream* output;
    Level key;
    Level lock;
    std::mutex mutex_;
	static int count;
};

int Log::count = 0;

#endif