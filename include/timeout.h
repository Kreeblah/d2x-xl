/*
 * timeout.h
 *
 *  Created on: 23.02.2011
 *      Author: Dietfrid Mali
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#if defined (__APPLE__) && defined (__MACH__) && defined (USE_MAC_FRAMEWORKS)
# include <SDL/SDL.h>
#else
# include <SDL.h>
#endif

#ifndef G3_SLEEP
#	ifdef _WIN32
#		pragma pack(push)
#		pragma pack(8)
#		include <windows.h>
#		pragma pack(pop)
#		define	G3_SLEEP(_t)	Sleep (_t)
#	else
#		include <unistd.h>
#		define	G3_SLEEP(_t)	usleep ((_t) * 1000)
#	endif
#endif

class CTimeout {
	private:
		time_t	m_t0;
		time_t	m_duration;

	public:
		CTimeout (time_t duration = 1000, bool bImmediate = false) : m_duration(duration) { Start (-1, bImmediate); }

		inline void Setup (time_t duration) { m_duration = duration; }

		inline void Start (time_t t = -1, bool bImmediate = false) { 
			m_t0 = (t < 0) ? SDL_GetTicks () : t; 
			if (bImmediate)
				m_t0 -= m_duration + 1;
			}

		inline time_t Progress (void) { return SDL_GetTicks () - m_t0; }

		bool Expired (bool bRestart = true) {
			if (m_duration == 0)
				return true;
			if (m_duration < 0)
				return false;
			time_t t = SDL_GetTicks ();
			if (t - m_t0 < m_duration)
				return false;
			if (bRestart)
				Start (t);
			else
				m_t0 += m_duration;
			return true;
			}

		void Throttle (void) {
			time_t t = SDL_GetTicks ();
			time_t dt = t - m_t0;
			if (dt < m_duration) {
				G3_SLEEP (int32_t (m_duration - dt));
				m_t0 += m_duration;
				}
			else
				m_t0 = t;
			}

		inline time_t Duration (void) { return m_duration; }

		inline void Suspend (void) {
			if (m_duration > 0)
				m_duration = -m_duration;
			}

		inline void Resume (void) {
			if (m_duration < 0)
				m_duration = -m_duration;
			}

		inline bool Suspended (void) { return m_duration < 0; }
	};


#endif /* TIMEOUT_H_ */
