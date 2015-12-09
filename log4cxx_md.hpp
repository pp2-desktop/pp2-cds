#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

static LoggerPtr __root__logger__(Logger::getLogger("main"));
static LoggerPtr __ws__logger__(Logger::getLogger("ws"));
static LoggerPtr __payload__logger__(Logger::getLogger("payload"));

// 기본(루트) 로깅 시 사용
/*
#define logt(m) (LOG4CXX_TRACE(__root__logger__, m))
#define logd(m) (LOG4CXX_DEBUG(__root__logger__, m))
#define logi(m) (LOG4CXX_INFO (__root__logger__, m))
#define logw(m) (LOG4CXX_WARN (__root__logger__, m))
#define loge(m) (LOG4CXX_ERROR(__root__logger__, m))
#define logf(m) (LOG4CXX_FATAL(__root__logger__, m))
*/

#define ws_logt(m) (LOG4CXX_TRACE(__ws__logger__, m))
#define ws_logd(m) (LOG4CXX_DEBUG(__ws__logger__, m))
#define ws_logi(m) (LOG4CXX_INFO (__ws__logger__, m))
#define ws_logw(m) (LOG4CXX_WARN (__ws__logger__, m))
#define ws_loge(m) (LOG4CXX_ERROR(__ws__logger__, m))
#define ws_logf(m) (LOG4CXX_FATAL(__ws__logger__, m))

#define payload_logt(m) (LOG4CXX_TRACE(__payload__logger__, m))
#define payload_logd(m) (LOG4CXX_DEBUG(__payload__logger__, m))
#define payload_logi(m) (LOG4CXX_INFO (__payload__logger__, m))
#define payload_logw(m) (LOG4CXX_WARN (__payload__logger__, m))
#define payload_loge(m) (LOG4CXX_ERROR(__payload__logger__, m))
#define payload_logf(m) (LOG4CXX_FATAL(__payload__logger__, m))

class log4cxx_md {
public:
  log4cxx_md();
  ~log4cxx_md();

  void init();

  static log4cxx_md& get() {
    static log4cxx_md obj;
    return obj;
  }

};
