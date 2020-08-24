#pragma once

namespace mainframe {
	namespace networking {
		enum class SocketError {
			success,
			invalidSocket,
			invalidHostname,
			failed
		};
	}
}