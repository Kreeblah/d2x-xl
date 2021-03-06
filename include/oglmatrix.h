#ifndef _OGLMATRIX_H
#define _OGLMATRIX_H

#include <stdint.h>
#include <string.h>
#include "glew.h"

class COGLMatrix {
	private:
		double	m_data [16];
		GLfloat	m_dataf [16];

	public:
		inline COGLMatrix& operator= (const COGLMatrix& other) { 
			memcpy (m_data, other.m_data, sizeof (m_data)); 
			return *this;
			}

		inline COGLMatrix& operator= (const double other [16]) { 
			memcpy (m_data, other, sizeof (m_data)); 
			return *this;
			}

		COGLMatrix Inverse (void);

		COGLMatrix& Get (GLuint nMatrix, bool bInverse = false) { 
			glGetDoublev (nMatrix, (GLdouble*) m_data); 
			if (bInverse)
				*this = Inverse ();
			return *this;
			}

		COGLMatrix& Getf (GLuint nMatrix, bool bInverse = false) { 
			glGetFloatv (nMatrix, (GLfloat*) m_dataf); 
			if (bInverse)
				*this = Inverse ();
			return *this;
			}

		void Set (void) { glLoadMatrixd ((GLdouble*) m_data); }

		void Mul (void) { glMultMatrixd ((GLdouble*) m_data); }

		double& operator[] (int32_t i) { return m_data [i]; }

		GLfloat* ToFloat (void) {
			for (int32_t i = 0; i < 16; i++)
				m_dataf [i] = GLfloat (m_data [i]);
			return m_dataf;
			}

		GLdouble* ToDouble (void) {
			for (int32_t i = 0; i < 16; i++)
				m_data [i] = GLdouble (m_dataf [i]);
			return m_data;
			}

		COGLMatrix& operator* (double factor) {
			for (int32_t i = 0; i < 16; i++)
				m_data [i] *= factor;
			return *this;
			}

		double Det (COGLMatrix& other) { return m_data [0] * other [0] + m_data [1] * other [4] + m_data [2] * other [8] + m_data [3] * other [12]; }

		inline double *Data (void) { return m_data; }

		inline float *Dataf (void) { return m_dataf; }
	};

#endif //_OGLMATRIX_H