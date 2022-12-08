/* ------------------------------------------------------------
name: "FaustSynth"
Code generated with Faust 2.23.4 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>

 /************************** BEGIN UI.h **************************/
 /************************************************************************
  FAUST Architecture File
  Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
  ---------------------------------------------------------------------
  This Architecture section is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; If not, see <http://www.gnu.org/licenses/>.

  EXCEPTION : As a special exception, you may create a larger work
  that contains this FAUST architecture section and distribute
  that work under terms of your choice, so long as this FAUST
  architecture section is not modified.
  ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

  /*******************************************************************************
   * UI : Faust DSP User Interface
   * User Interface as expected by the buildUserInterface() method of a DSP.
   * This abstract class contains only the method that the Faust compiler can
   * generate to describe a DSP user interface.
   ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}

    // -- widget's layouts

    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;

    // -- active widgets

    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;

    // -- passive widgets

    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;

    // -- soundfiles

    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

    // -- metadata declarations

    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

 /*******************************************************************************
  * PathBuilder : Faust User Interface
  * Helper class to build complete hierarchical path for UI items.
  ******************************************************************************/

class PathBuilder
{

protected:

    std::vector<std::string> fControlsLevel;

public:

    PathBuilder() {}
    virtual ~PathBuilder() {}

    std::string buildPath(const std::string& label)
    {
        std::string res = "/";
        for (size_t i = 0; i < fControlsLevel.size(); i++) {
            res += fControlsLevel[i];
            res += "/";
        }
        res += label;
        std::replace(res.begin(), res.end(), ' ', '_');
        return res;
    }

    void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
    void popLabel() { fControlsLevel.pop_back(); }

};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{

protected:

    // Complete path map
    std::map<std::string, FAUSTFLOAT*> fPathZoneMap;

    // Label zone map
    std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;

public:

    MapUI() {}
    virtual ~MapUI() {}

    // -- widget's layouts
    void openTabBox(const char* label)
    {
        pushLabel(label);
    }
    void openHorizontalBox(const char* label)
    {
        pushLabel(label);
    }
    void openVerticalBox(const char* label)
    {
        pushLabel(label);
    }
    void closeBox()
    {
        popLabel();
    }

    // -- active widgets
    void addButton(const char* label, FAUSTFLOAT* zone)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addCheckButton(const char* label, FAUSTFLOAT* zone)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }

    // -- passive widgets
    void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }

    // -- soundfiles
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

    // -- metadata declarations
    void declare(FAUSTFLOAT* zone, const char* key, const char* val)
    {}

    // set/get
    void setParamValue(const std::string& path, FAUSTFLOAT value)
    {
        if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
            *fPathZoneMap[path] = value;
        }
        else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
            *fLabelZoneMap[path] = value;
        }
    }

    FAUSTFLOAT getParamValue(const std::string& path)
    {
        if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
            return *fPathZoneMap[path];
        }
        else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
            return *fLabelZoneMap[path];
        }
        else {
            return FAUSTFLOAT(0);
        }
    }

    // map access 
    std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }

    int getParamsCount() { return int(fPathZoneMap.size()); }

    std::string getParamAddress(int index)
    {
        std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
        while (index-- > 0 && it++ != fPathZoneMap.end()) {}
        return (*it).first;
    }

    std::string getParamAddress(FAUSTFLOAT* zone)
    {
        std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
        do {
            if ((*it).second == zone) return (*it).first;
        } while (it++ != fPathZoneMap.end());
        return "";
    }

    static bool endsWith(std::string const& str, std::string const& end)
    {
        size_t l1 = str.length();
        size_t l2 = end.length();
        return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
    }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;

};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {

    virtual ~dsp_memory_manager() {}

    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;

};

/**
* Signal processor definition.
*/

class dsp {

public:

    dsp() {}
    virtual ~dsp() {}

    /* Return instance number of audio inputs */
    virtual int getNumInputs() = 0;

    /* Return instance number of audio outputs */
    virtual int getNumOutputs() = 0;

    /**
     * Trigger the ui_interface parameter with instance specific calls
     * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
     *
     * @param ui_interface - the user interface builder
     */
    virtual void buildUserInterface(UI* ui_interface) = 0;

    /* Returns the sample rate currently used by the instance */
    virtual int getSampleRate() = 0;

    /**
     * Global init, calls the following methods:
     * - static class 'classInit': static tables initialization
     * - 'instanceInit': constants and instance state initialization
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void init(int sample_rate) = 0;

    /**
     * Init instance state
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void instanceInit(int sample_rate) = 0;

    /**
     * Init instance constant state
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void instanceConstants(int sample_rate) = 0;

    /* Init default control parameters values */
    virtual void instanceResetUserInterface() = 0;

    /* Init instance state (delay lines...) */
    virtual void instanceClear() = 0;

    /**
     * Return a clone of the instance.
     *
     * @return a copy of the instance on success, otherwise a null pointer.
     */
    virtual dsp* clone() = 0;

    /**
     * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
     *
     * @param m - the Meta* meta user
     */
    virtual void metadata(Meta* m) = 0;

    /**
     * DSP instance computation, to be called with successive in/out audio buffers.
     *
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
     * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
     *
     */
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;

    /**
     * DSP instance computation: alternative method to be used by subclasses.
     *
     * @param date_usec - the timestamp in microsec given by audio driver.
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
     * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
     *
     */
    virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

protected:

    dsp* fDSP;

public:

    decorator_dsp(dsp* dsp = nullptr) :fDSP(dsp) {}
    virtual ~decorator_dsp() { delete fDSP; }

    virtual int getNumInputs() { return fDSP->getNumInputs(); }
    virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
    virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
    virtual int getSampleRate() { return fDSP->getSampleRate(); }
    virtual void init(int sample_rate) { fDSP->init(sample_rate); }
    virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
    virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
    virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
    virtual void instanceClear() { fDSP->instanceClear(); }
    virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
    virtual void metadata(Meta* m) { fDSP->metadata(m); }
    // Beware: subclasses usually have to overload the two 'compute' methods
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
    virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }

};

/**
 * DSP factory class.
 */

class dsp_factory {

protected:

    // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
    virtual ~dsp_factory() {}

public:

    virtual std::string getName() = 0;
    virtual std::string getSHAKey() = 0;
    virtual std::string getDSPCode() = 0;
    virtual std::string getCompileOptions() = 0;
    virtual std::vector<std::string> getLibraryList() = 0;
    virtual std::vector<std::string> getIncludePathnames() = 0;

    virtual dsp* createDSPInstance() = 0;

    virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
    virtual dsp_memory_manager* getMemoryManager() = 0;

};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
#include <xmmintrin.h>
#ifdef __SSE2__
#define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
#else
#define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
#endif
#else
#define AVOIDDENORMALS
#endif

#endif
 /**************************  END  dsp.h **************************/

 // BEGIN-FAUSTDSP

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class mydspSIG0 {

private:

	int iVec0[2];
	int iRec1[2];

public:

	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}

	void instanceInitmydspSIG0(int sample_rate) {
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iVec0[l1] = 0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iRec1[l2] = 0;
		}
	}

	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec1[0] = (iVec0[1] + iRec1[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec1[0]));
			iVec0[1] = iVec0[0];
			iRec1[1] = iRec1[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];
static float mydsp_faustpower2_f(float value) {
	return value * value;
}

class mydsp : public dsp {

private:

	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fConst4;
	float fRec2[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	int iRec4[2];
	FAUSTFLOAT fHslider5;
	float fRec5[2];
	float fRec6[2];
	FAUSTFLOAT fHslider6;
	float fRec3[3];
	float fRec8[3];
	float fRec9[3];
	float fRec10[3];
	float fRec11[3];
	FAUSTFLOAT fButton0;
	float fRec12[2];

public:

	void metadata(Meta* m) {
		m->declare("basics_lib_listInterp_author", "Romain Michon");
		m->declare("basics_lib_name", "Faust Basic Element Library");
		m->declare("basics_lib_version", "0.8");
		m->declare("compile_options", "-lang cpp -es 1 -mcd 16 -single -ftz 0");
		m->declare("copyright", "(c)Romain Michon, CCRMA (Stanford University), GRAME");
		m->declare("description", "Simple source-filter vocal synthesizer.");
		m->declare("filename", "untitled.dsp");
		m->declare("filters_lib_fir_author", "Julius O. Smith III");
		m->declare("filters_lib_fir_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_fir_license", "MIT-style STK-4.3 license");
		m->declare("filters_lib_iir_author", "Julius O. Smith III");
		m->declare("filters_lib_iir_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_iir_license", "MIT-style STK-4.3 license");
		m->declare("filters_lib_lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters_lib_name", "Faust Filters Library");
		m->declare("filters_lib_resonbp_author", "Julius O. Smith III");
		m->declare("filters_lib_resonbp_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_resonbp_license", "MIT-style STK-4.3 license");
		m->declare("filters_lib_tf2_author", "Julius O. Smith III");
		m->declare("filters_lib_tf2_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_tf2_license", "MIT-style STK-4.3 license");
		m->declare("filters_lib_tf2s_author", "Julius O. Smith III");
		m->declare("filters_lib_tf2s_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_tf2s_license", "MIT-style STK-4.3 license");
		m->declare("filters_lib_version", "0.3");
		m->declare("library_path0", "/libraries/stdfaust.lib");
		m->declare("library_path1", "/libraries/physmodels.lib");
		m->declare("library_path2", "/libraries/oscillators.lib");
		m->declare("library_path3", "/libraries/platform.lib");
		m->declare("library_path4", "/libraries/maths.lib");
		m->declare("library_path5", "/libraries/basics.lib");
		m->declare("library_path6", "/libraries/signals.lib");
		m->declare("library_path7", "/libraries/noises.lib");
		m->declare("library_path8", "/libraries/filters.lib");
		m->declare("license", "MIT");
		m->declare("maths_lib_author", "GRAME");
		m->declare("maths_lib_copyright", "GRAME");
		m->declare("maths_lib_license", "LGPL with exception");
		m->declare("maths_lib_name", "Faust Math Library");
		m->declare("maths_lib_version", "2.5");
		m->declare("name", "untitled");
		m->declare("noises_lib_name", "Faust Noise Generator Library");
		m->declare("noises_lib_version", "0.4");
		m->declare("oscillators_lib_name", "Faust Oscillator Library");
		m->declare("oscillators_lib_version", "0.3");
		m->declare("physmodels_lib_SFFormantModel_author", "Mike Olsen");
		m->declare("physmodels_lib_autobendFreq_author", "Mike Olsen");
		m->declare("physmodels_lib_formantFilterbank_author", "Mike Olsen");
		m->declare("physmodels_lib_name", "Faust Physical Models Library");
		m->declare("physmodels_lib_version", "0.1");
		m->declare("physmodels_lib_vocalEffort_author", "Mike Olsen");
		m->declare("physmodels_lib_voiceGender_author", "Mike Olsen");
		m->declare("platform_lib_name", "Generic Platform Library");
		m->declare("platform_lib_version", "0.2");
		m->declare("signals_lib_name", "Faust Signal Routing Library");
		m->declare("signals_lib_version", "0.3");
		m->declare("version", "2.52.3");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}

	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}

	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 3.1415927f / fConst0;
		fConst4 = 1.0f / fConst0;
	}

	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(6.0f);
		fHslider3 = FAUSTFLOAT(0.5f);
		fHslider4 = FAUSTFLOAT(4.4e+02f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.9f);
		fButton0 = FAUSTFLOAT(0.0f);
	}

	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec4[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 3; l7 = l7 + 1) {
			fRec3[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec8[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec9[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec10[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec11[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec12[l12] = 0.0f;
		}
	}

	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}

	virtual mydsp* clone() {
		return new mydsp();
	}

	virtual int getSampleRate() {
		return fSampleRate;
	}

	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("untitled");
		ui_interface->addButton("gate", &fButton0);
		ui_interface->openVerticalBox("vocal");
		ui_interface->declare(&fHslider4, "0", "");
		ui_interface->addHorizontalSlider("freq", &fHslider4, FAUSTFLOAT(4.4e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider6, "1", "");
		ui_interface->addHorizontalSlider("gain", &fHslider6, FAUSTFLOAT(0.9f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider1, "2", "");
		ui_interface->addHorizontalSlider("voiceType", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider0, "3", "");
		ui_interface->addHorizontalSlider("vowel", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider5, "4", "");
		ui_interface->addHorizontalSlider("fricative", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider2, "5", "");
		ui_interface->addHorizontalSlider("vibratoFreq", &fHslider2, FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "6", "");
		ui_interface->addHorizontalSlider("vibratoGain", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
		ui_interface->closeBox();
	}

	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = fConst1 * float(fHslider0);
		float fSlow1 = float(fHslider1);
		float fSlow2 = 5.0f * fSlow1;
		float fSlow3 = 5.0f * (1.0f - fSlow1);
		int iThen76 = ((fSlow1 == 3.0f) ? 1 : 0);
		int iSlow4 = ((fSlow1 == 0.0f) ? 1 : iThen76) == 0;
		float fSlow5 = fConst4 * float(fHslider2);
		float fSlow6 = 0.1f * float(fHslider3);
		float fSlow7 = float(fHslider4);
		float fSlow8 = fConst1 * float(fHslider5);
		float fSlow9 = float(fHslider6);
		int iSlow10 = fSlow1 != 2.0f;
		float fSlow11 = 2.0f * fSlow7;
		float fSlow12 = fConst1 * float(fButton0);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow0 + fConst2 * fRec0[1];
			float fTemp0 = fSlow2 + fRec0[0];
			int iTemp1 = fTemp0 < 23.0f;
			int iTemp2 = fTemp0 < 24.0f;
			float fTemp3 = fSlow2 + fRec0[0] + -23.0f;
			float fElse0 = 3e+02f * fTemp3 + 3e+03f;
			int iTemp4 = fTemp0 < 22.0f;
			float fTemp5 = fSlow2 + fRec0[0] + -22.0f;
			float fElse1 = 3.54e+03f - 5.4e+02f * fTemp5;
			int iTemp6 = fTemp0 < 21.0f;
			float fTemp7 = fSlow2 + fRec0[0] + -21.0f;
			float fElse2 = 3.58e+03f - 4e+01f * fTemp7;
			int iTemp8 = fTemp0 < 2e+01f;
			float fTemp9 = fSlow2 + fRec0[0] + -2e+01f;
			float fElse3 = 3.3e+02f * fTemp9 + 3.25e+03f;
			int iTemp10 = fTemp0 < 19.0f;
			float fTemp11 = fSlow2 + fRec0[0] + -19.0f;
			float fElse4 = 4.95e+03f - 1.7e+03f * fTemp11;
			int iTemp12 = fTemp0 < 18.0f;
			int iTemp13 = fTemp0 < 17.0f;
			int iTemp14 = fTemp0 < 16.0f;
			int iTemp15 = fTemp0 < 15.0f;
			int iTemp16 = fTemp0 < 14.0f;
			float fTemp17 = fSlow2 + fRec0[0] + -14.0f;
			float fElse5 = 1.55e+03f * fTemp17 + 3.4e+03f;
			int iTemp18 = fTemp0 < 13.0f;
			float fTemp19 = fSlow2 + fRec0[0] + -13.0f;
			float fElse6 = 1e+02f * fTemp19 + 3.3e+03f;
			int iTemp20 = fTemp0 < 12.0f;
			float fTemp21 = fSlow2 + fRec0[0] + -12.0f;
			float fElse7 = 3.59e+03f - 2.9e+02f * fTemp21;
			int iTemp22 = fTemp0 < 11.0f;
			float fTemp23 = fSlow2 + fRec0[0] + -11.0f;
			float fElse8 = 2.9e+02f * fTemp23 + 3.3e+03f;
			int iTemp24 = fTemp0 < 1e+01f;
			float fTemp25 = fSlow2 + fRec0[0] + -1e+01f;
			float fTemp26 = 5e+01f * fTemp25;
			float fElse9 = 3.35e+03f - fTemp26;
			int iTemp27 = fTemp0 < 9.0f;
			float fTemp28 = fSlow2 + fRec0[0] + -9.0f;
			float fElse10 = 4e+02f * fTemp28 + 2.95e+03f;
			int iTemp29 = fTemp0 < 8.0f;
			float fTemp30 = fSlow2 + fRec0[0] + -8.0f;
			float fTemp31 = 5e+01f * fTemp30;
			float fElse11 = fTemp31 + 2.9e+03f;
			int iTemp32 = fTemp0 < 7.0f;
			float fTemp33 = fSlow2 + fRec0[0] + -7.0f;
			float fElse12 = 3.34e+03f - 4.4e+02f * fTemp33;
			int iTemp34 = fTemp0 < 6.0f;
			float fTemp35 = fSlow2 + fRec0[0] + -6.0f;
			float fElse13 = 2.4e+02f * fTemp35 + 3.1e+03f;
			int iTemp36 = fTemp0 < 5.0f;
			float fTemp37 = fRec0[0] - fSlow3;
			float fTemp38 = 3.5e+02f * fTemp37;
			float fElse14 = fTemp38 + 2.75e+03f;
			int iTemp39 = fTemp0 < 4.0f;
			float fTemp40 = fSlow2 + fRec0[0] + -4.0f;
			float fElse15 = 4.95e+03f - 2.2e+03f * fTemp40;
			float fThen16 = ((iTemp36) ? fElse15 : 2.75e+03f);
			float fThen17 = ((iTemp34) ? fElse14 : 3.1e+03f);
			float fElse17 = ((iTemp39) ? 4.95e+03f : fThen16);
			float fThen18 = ((iTemp32) ? fElse13 : 3.34e+03f);
			float fElse18 = ((iTemp36) ? fElse17 : fThen17);
			float fThen19 = ((iTemp29) ? fElse12 : 2.9e+03f);
			float fElse19 = ((iTemp34) ? fElse18 : fThen18);
			float fThen20 = ((iTemp27) ? fElse11 : 2.95e+03f);
			float fElse20 = ((iTemp32) ? fElse19 : fThen19);
			float fThen21 = ((iTemp24) ? fElse10 : 3.35e+03f);
			float fElse21 = ((iTemp29) ? fElse20 : fThen20);
			float fThen22 = ((iTemp22) ? fElse9 : 3.3e+03f);
			float fElse22 = ((iTemp27) ? fElse21 : fThen21);
			float fThen23 = ((iTemp20) ? fElse8 : 3.59e+03f);
			float fElse23 = ((iTemp24) ? fElse22 : fThen22);
			float fThen24 = ((iTemp18) ? fElse7 : 3.3e+03f);
			float fElse24 = ((iTemp22) ? fElse23 : fThen23);
			float fThen25 = ((iTemp16) ? fElse6 : 3.4e+03f);
			float fElse25 = ((iTemp20) ? fElse24 : fThen24);
			float fThen26 = ((iTemp15) ? fElse5 : 4.95e+03f);
			float fElse26 = ((iTemp18) ? fElse25 : fThen25);
			float fElse27 = ((iTemp16) ? fElse26 : fThen26);
			float fElse28 = ((iTemp15) ? fElse27 : 4.95e+03f);
			float fElse29 = ((iTemp14) ? fElse28 : 4.95e+03f);
			float fElse30 = ((iTemp13) ? fElse29 : 4.95e+03f);
			float fThen31 = ((iTemp8) ? fElse4 : 3.25e+03f);
			float fElse31 = ((iTemp12) ? fElse30 : 4.95e+03f);
			float fThen32 = ((iTemp6) ? fElse3 : 3.58e+03f);
			float fElse32 = ((iTemp10) ? fElse31 : fThen31);
			float fThen33 = ((iTemp4) ? fElse2 : 3.54e+03f);
			float fElse33 = ((iTemp8) ? fElse32 : fThen32);
			float fThen34 = ((iTemp1) ? fElse1 : 3e+03f);
			float fElse34 = ((iTemp6) ? fElse33 : fThen33);
			float fThen35 = ((iTemp2) ? fElse0 : 3.3e+03f);
			float fElse35 = ((iTemp4) ? fElse34 : fThen34);
			float fTemp41 = ((iTemp1) ? fElse35 : fThen35);
			float fTemp42 = std::tan(fConst3 * fTemp41);
			float fElse36 = 135.0f - 15.0f * fTemp3;
			float fElse37 = 15.0f * fTemp5 + 1.2e+02f;
			float fTemp43 = 2e+01f * fTemp9;
			float fElse38 = 1.4e+02f - fTemp43;
			float fElse39 = 2e+02f - 6e+01f * fTemp11;
			float fTemp44 = fSlow2 + fRec0[0] + -18.0f;
			float fElse40 = 8e+01f * fTemp44 + 1.2e+02f;
			float fTemp45 = fSlow2 + fRec0[0] + -16.0f;
			float fTemp46 = 8e+01f * fTemp45;
			float fElse41 = 2e+02f - fTemp46;
			float fTemp47 = fSlow2 + fRec0[0] + -15.0f;
			float fElse42 = 6e+01f * fTemp47 + 1.4e+02f;
			float fTemp48 = 2e+01f * fTemp17;
			float fElse43 = fTemp48 + 1.2e+02f;
			float fTemp49 = 2e+01f * fTemp25;
			float fElse44 = 1.4e+02f - fTemp49;
			float fTemp50 = 2e+01f * fTemp28;
			float fElse45 = fTemp50 + 1.2e+02f;
			float fTemp51 = 1e+01f * fTemp37;
			float fElse46 = 1.3e+02f - fTemp51;
			float fElse47 = 2e+02f - 7e+01f * fTemp40;
			int iTemp52 = fTemp0 < 3.0f;
			float fTemp53 = fSlow2 + fRec0[0] + -3.0f;
			float fElse48 = 65.0f * fTemp53 + 135.0f;
			int iTemp54 = fTemp0 < 2.0f;
			float fTemp55 = fSlow2 + fRec0[0] + -2.0f;
			float fElse49 = 2e+02f - 65.0f * fTemp55;
			int iTemp56 = fTemp0 < 1.0f;
			int iTemp57 = fTemp0 < 0.0f;
			float fElse50 = 6e+01f * fTemp0 + 1.4e+02f;
			float fThen51 = ((iTemp56) ? fElse50 : 2e+02f);
			float fElse52 = ((iTemp57) ? 1.4e+02f : fThen51);
			float fThen53 = ((iTemp52) ? fElse49 : 135.0f);
			float fElse53 = ((iTemp56) ? fElse52 : 2e+02f);
			float fThen54 = ((iTemp39) ? fElse48 : 2e+02f);
			float fElse54 = ((iTemp54) ? fElse53 : fThen53);
			float fThen55 = ((iTemp36) ? fElse47 : 1.3e+02f);
			float fElse55 = ((iTemp52) ? fElse54 : fThen54);
			float fThen56 = ((iTemp34) ? fElse46 : 1.2e+02f);
			float fElse56 = ((iTemp39) ? fElse55 : fThen55);
			float fElse57 = ((iTemp36) ? fElse56 : fThen56);
			float fElse58 = ((iTemp34) ? fElse57 : 1.2e+02f);
			float fElse59 = ((iTemp32) ? fElse58 : 1.2e+02f);
			float fThen60 = ((iTemp24) ? fElse45 : 1.4e+02f);
			float fElse60 = ((iTemp29) ? fElse59 : 1.2e+02f);
			float fThen61 = ((iTemp22) ? fElse44 : 1.2e+02f);
			float fElse61 = ((iTemp27) ? fElse60 : fThen60);
			float fElse62 = ((iTemp24) ? fElse61 : fThen61);
			float fElse63 = ((iTemp22) ? fElse62 : 1.2e+02f);
			float fElse64 = ((iTemp20) ? fElse63 : 1.2e+02f);
			float fThen65 = ((iTemp15) ? fElse43 : 1.4e+02f);
			float fElse65 = ((iTemp18) ? fElse64 : 1.2e+02f);
			float fThen66 = ((iTemp14) ? fElse42 : 2e+02f);
			float fElse66 = ((iTemp16) ? fElse65 : fThen65);
			float fThen67 = ((iTemp13) ? fElse41 : 1.2e+02f);
			float fElse67 = ((iTemp15) ? fElse66 : fThen66);
			float fElse68 = ((iTemp14) ? fElse67 : fThen67);
			float fThen69 = ((iTemp10) ? fElse40 : 2e+02f);
			float fElse69 = ((iTemp13) ? fElse68 : 1.2e+02f);
			float fThen70 = ((iTemp8) ? fElse39 : 1.4e+02f);
			float fElse70 = ((iTemp12) ? fElse69 : fThen69);
			float fThen71 = ((iTemp6) ? fElse38 : 1.2e+02f);
			float fElse71 = ((iTemp10) ? fElse70 : fThen70);
			float fElse72 = ((iTemp8) ? fElse71 : fThen71);
			float fThen73 = ((iTemp1) ? fElse37 : 135.0f);
			float fElse73 = ((iTemp6) ? fElse72 : 1.2e+02f);
			float fThen74 = ((iTemp2) ? fElse36 : 1.2e+02f);
			float fElse74 = ((iTemp4) ? fElse73 : fThen73);
			float fTemp58 = ((iTemp1) ? fElse74 : fThen74) / fTemp41;
			float fTemp59 = 1.0f / fTemp42;
			float fTemp60 = (fTemp59 + fTemp58) / fTemp42 + 1.0f;
			float fTemp61 = fTemp42 * fTemp60;
			float fElse77 = 0.018496f * fTemp5 + 0.031623f;
			float fElse78 = 0.1f - 0.068377f * fTemp7;
			float fElse79 = 0.020567f * fTemp9 + 0.079433f;
			float fElse80 = 0.078433f * fTemp11 + 0.001f;
			float fElse81 = 0.003162f - 0.002162f * fTemp44;
			float fTemp62 = fSlow2 + fRec0[0] + -17.0f;
			float fElse82 = 0.00631f - 0.003148f * fTemp62;
			float fElse83 = 0.004725f * fTemp45 + 0.001585f;
			float fElse84 = 0.003162f - 0.001577f * fTemp47;
			float fElse85 = 0.019953f - 0.016791f * fTemp17;
			float fElse86 = 0.004104f * fTemp21 + 0.015849f;
			float fElse87 = 0.1f - 0.084151f * fTemp23;
			float fTemp63 = ((iTemp20) ? fElse87 : 0.015849f);
			float fElse88 = 0.087411f * fTemp25 + 0.012589f;
			float fElse89 = 0.015849f - 0.00326f * fTemp28;
			float fElse90 = 0.005849f * fTemp30 + 0.01f;
			float fElse91 = 0.039811f - 0.029811f * fTemp33;
			float fElse92 = 0.125893f - 0.086082f * fTemp35;
			float fElse93 = 0.025893f * fTemp37 + 0.1f;
			float fElse94 = 0.099369f * fTemp40 + 0.000631f;
			float fElse95 = 0.001778f - 0.001147f * fTemp53;
			float fElse96 = 0.000778f * fTemp55 + 0.001f;
			float fThen97 = ((iTemp52) ? fElse96 : 0.001778f);
			float fThen98 = ((iTemp39) ? fElse95 : 0.000631f);
			float fElse98 = ((iTemp54) ? 0.001f : fThen97);
			float fThen99 = ((iTemp36) ? fElse94 : 0.1f);
			float fElse99 = ((iTemp52) ? fElse98 : fThen98);
			float fThen100 = ((iTemp34) ? fElse93 : 0.125893f);
			float fElse100 = ((iTemp39) ? fElse99 : fThen99);
			float fThen101 = ((iTemp32) ? fElse92 : 0.039811f);
			float fElse101 = ((iTemp36) ? fElse100 : fThen100);
			float fThen102 = ((iTemp29) ? fElse91 : 0.01f);
			float fElse102 = ((iTemp34) ? fElse101 : fThen101);
			float fThen103 = ((iTemp27) ? fElse90 : 0.015849f);
			float fElse103 = ((iTemp32) ? fElse102 : fThen102);
			float fThen104 = ((iTemp24) ? fElse89 : 0.012589f);
			float fElse104 = ((iTemp29) ? fElse103 : fThen103);
			float fThen105 = ((iTemp22) ? fElse88 : 0.1f);
			float fElse105 = ((iTemp27) ? fElse104 : fThen104);
			float fElse106 = ((iTemp24) ? fElse105 : fThen105);
			float fThen107 = ((iTemp18) ? fElse86 : 0.019953f);
			float fElse107 = ((iTemp22) ? fElse106 : fTemp63);
			float fElse108 = ((iTemp20) ? fElse107 : fThen107);
			float fThen109 = ((iTemp15) ? fElse85 : 0.003162f);
			float fElse109 = ((iTemp18) ? fElse108 : 0.019953f);
			float fThen110 = ((iTemp14) ? fElse84 : 0.001585f);
			float fElse110 = ((iTemp16) ? fElse109 : fThen109);
			float fThen111 = ((iTemp13) ? fElse83 : 0.00631f);
			float fElse111 = ((iTemp15) ? fElse110 : fThen110);
			float fThen112 = ((iTemp12) ? fElse82 : 0.003162f);
			float fElse112 = ((iTemp14) ? fElse111 : fThen111);
			float fThen113 = ((iTemp10) ? fElse81 : 0.001f);
			float fElse113 = ((iTemp13) ? fElse112 : fThen112);
			float fThen114 = ((iTemp8) ? fElse80 : 0.079433f);
			float fElse114 = ((iTemp12) ? fElse113 : fThen113);
			float fThen115 = ((iTemp6) ? fElse79 : 0.1f);
			float fElse115 = ((iTemp10) ? fElse114 : fThen114);
			float fThen116 = ((iTemp4) ? fElse78 : 0.031623f);
			float fElse116 = ((iTemp8) ? fElse115 : fThen115);
			float fThen117 = ((iTemp1) ? fElse77 : 0.050119f);
			float fElse117 = ((iTemp6) ? fElse116 : fThen116);
			float fElse118 = ((iTemp4) ? fElse117 : fThen117);
			float fTemp64 = ((iTemp1) ? fElse118 : 0.050119f);
			fRec2[0] = fSlow5 + (fRec2[1] - std::floor(fSlow5 + fRec2[1]));
			float fTemp65 = fSlow6 * ftbl0mydspSIG0[int(65536.0f * fRec2[0])] + 1.0f;
			float fTemp66 = fSlow7 * fTemp65;
			float fTemp67 = 0.00084f * (1e+03f - fTemp66) + 0.8f;
			float fTemp68 = 0.0036666666f * (4e+02f - fTemp66) + 3.0f;
			float fThen119 = fTemp67 * fTemp64;
			float fElse119 = fTemp68 * fTemp64;
			float fTemp69 = ((iSlow4) ? fElse119 : fThen119);
			iRec4[0] = 1103515245 * iRec4[1] + 12345;
			fRec5[0] = fSlow8 + fConst2 * fRec5[1];
			float fTemp70 = std::max<float>(1.1920929e-07f, std::fabs(fTemp66));
			float fTemp71 = fRec6[1] + fConst4 * fTemp70;
			float fTemp72 = fTemp71 + -1.0f;
			int iTemp73 = fTemp72 < 0.0f;
			fRec6[0] = ((iTemp73) ? fTemp71 : fTemp72);
			float fThen121 = fTemp71 + fTemp72 * (1.0f - fConst0 / fTemp70);
			float fRec7 = ((iTemp73) ? fTemp71 : fThen121);
			float fTemp74 = fSlow9 * ((1.0f - fRec5[0]) * (2.0f * fRec7 + -1.0f) + 4.656613e-10f * fRec5[0] * float(iRec4[0]));
			fRec3[0] = fTemp74 - (fRec3[2] * ((fTemp59 - fTemp58) / fTemp42 + 1.0f) + 2.0f * fRec3[1] * (1.0f - 1.0f / mydsp_faustpower2_f(fTemp42))) / fTemp60;
			float fTemp75 = 1e+02f * fTemp3;
			float fElse122 = fTemp75 + 2.8e+03f;
			float fElse123 = 3.25e+03f - 4.5e+02f * fTemp5;
			float fElse124 = 5e+01f * fTemp7 + 3.2e+03f;
			float fElse125 = 3e+02f * fTemp9 + 2.9e+03f;
			float fElse126 = 3.8e+03f - 9e+02f * fTemp11;
			float fElse127 = 3.9e+03f - 1e+02f * fTemp62;
			float fElse128 = 3e+02f * fTemp45 + 3.6e+03f;
			float fElse129 = 3.9e+03f - 3e+02f * fTemp47;
			float fElse130 = 9e+02f * fTemp17 + 3e+03f;
			float fElse131 = 3.35e+03f - 3.5e+02f * fTemp21;
			float fElse132 = 3.5e+02f * fTemp23 + 3e+03f;
			float fElse133 = 325.0f * fTemp28 + 2675.0f;
			float fElse134 = 75.0f * fTemp30 + 2.6e+03f;
			float fElse135 = 3.05e+03f - 4.5e+02f * fTemp33;
			float fElse136 = 2.5e+02f * fTemp35 + 2.8e+03f;
			float fElse137 = fTemp38 + 2.45e+03f;
			float fElse138 = 3.5e+03f - 1.05e+03f * fTemp40;
			float fElse139 = 3.7e+03f - 2e+02f * fTemp55;
			float fTemp76 = fSlow2 + fRec0[0] + -1.0f;
			float fElse140 = 4e+02f * fTemp76 + 3.3e+03f;
			float fElse141 = 3.5e+03f - 2e+02f * fTemp0;
			float fThen142 = ((iTemp56) ? fElse141 : 3.3e+03f);
			float fThen143 = ((iTemp54) ? fElse140 : 3.7e+03f);
			float fElse143 = ((iTemp57) ? 3.5e+03f : fThen142);
			float fThen144 = ((iTemp52) ? fElse139 : 3.5e+03f);
			float fElse144 = ((iTemp56) ? fElse143 : fThen143);
			float fElse145 = ((iTemp54) ? fElse144 : fThen144);
			float fThen146 = ((iTemp36) ? fElse138 : 2.45e+03f);
			float fElse146 = ((iTemp52) ? fElse145 : 3.5e+03f);
			float fThen147 = ((iTemp34) ? fElse137 : 2.8e+03f);
			float fElse147 = ((iTemp39) ? fElse146 : fThen146);
			float fThen148 = ((iTemp32) ? fElse136 : 3.05e+03f);
			float fElse148 = ((iTemp36) ? fElse147 : fThen147);
			float fThen149 = ((iTemp29) ? fElse135 : 2.6e+03f);
			float fElse149 = ((iTemp34) ? fElse148 : fThen148);
			float fThen150 = ((iTemp27) ? fElse134 : 2675.0f);
			float fElse150 = ((iTemp32) ? fElse149 : fThen149);
			float fThen151 = ((iTemp24) ? fElse133 : 3e+03f);
			float fElse151 = ((iTemp29) ? fElse150 : fThen150);
			float fElse152 = ((iTemp27) ? fElse151 : fThen151);
			float fThen153 = ((iTemp20) ? fElse132 : 3.35e+03f);
			float fElse153 = ((iTemp24) ? fElse152 : 3e+03f);
			float fThen154 = ((iTemp18) ? fElse131 : 3e+03f);
			float fElse154 = ((iTemp22) ? fElse153 : fThen153);
			float fElse155 = ((iTemp20) ? fElse154 : fThen154);
			float fThen156 = ((iTemp15) ? fElse130 : 3.9e+03f);
			float fElse156 = ((iTemp18) ? fElse155 : 3e+03f);
			float fThen157 = ((iTemp14) ? fElse129 : 3.6e+03f);
			float fElse157 = ((iTemp16) ? fElse156 : fThen156);
			float fThen158 = ((iTemp13) ? fElse128 : 3.9e+03f);
			float fElse158 = ((iTemp15) ? fElse157 : fThen157);
			float fThen159 = ((iTemp12) ? fElse127 : 3.8e+03f);
			float fElse159 = ((iTemp14) ? fElse158 : fThen158);
			float fElse160 = ((iTemp13) ? fElse159 : fThen159);
			float fThen161 = ((iTemp8) ? fElse126 : 2.9e+03f);
			float fElse161 = ((iTemp12) ? fElse160 : 3.8e+03f);
			float fThen162 = ((iTemp6) ? fElse125 : 3.2e+03f);
			float fElse162 = ((iTemp10) ? fElse161 : fThen161);
			float fThen163 = ((iTemp4) ? fElse124 : 3.25e+03f);
			float fElse163 = ((iTemp8) ? fElse162 : fThen162);
			float fThen164 = ((iTemp1) ? fElse123 : 2.8e+03f);
			float fElse164 = ((iTemp6) ? fElse163 : fThen163);
			float fThen165 = ((iTemp2) ? fElse122 : 2.9e+03f);
			float fElse165 = ((iTemp4) ? fElse164 : fThen164);
			float fTemp77 = ((iTemp1) ? fElse165 : fThen165);
			float fTemp78 = std::tan(fConst3 * fTemp77);
			float fElse166 = 1.3e+02f - 1e+01f * fTemp3;
			float fTemp79 = 1e+01f * fTemp5;
			float fElse167 = fTemp79 + 1.2e+02f;
			float fTemp80 = 1e+01f * fTemp9;
			float fElse168 = 1.3e+02f - fTemp80;
			float fTemp81 = 5e+01f * fTemp11;
			float fElse169 = 1.8e+02f - fTemp81;
			float fElse170 = 6e+01f * fTemp44 + 1.2e+02f;
			float fElse171 = 1.5e+02f - 3e+01f * fTemp45;
			float fTemp82 = 2e+01f * fTemp47;
			float fElse172 = fTemp82 + 1.3e+02f;
			float fElse173 = 1e+01f * fTemp17 + 1.2e+02f;
			float fTemp83 = 1e+01f * fTemp25;
			float fElse174 = 1.3e+02f - fTemp83;
			float fTemp84 = 1e+01f * fTemp28;
			float fElse175 = fTemp84 + 1.2e+02f;
			float fTemp85 = 6e+01f * fTemp40;
			float fElse176 = 1.8e+02f - fTemp85;
			float fElse177 = 5e+01f * fTemp53 + 1.3e+02f;
			float fTemp86 = 2e+01f * fTemp55;
			float fElse178 = 1.5e+02f - fTemp86;
			float fTemp87 = 2e+01f * fTemp0;
			float fElse179 = fTemp87 + 1.3e+02f;
			float fThen180 = ((iTemp56) ? fElse179 : 1.5e+02f);
			float fElse181 = ((iTemp57) ? 1.3e+02f : fThen180);
			float fThen182 = ((iTemp52) ? fElse178 : 1.3e+02f);
			float fElse182 = ((iTemp56) ? fElse181 : 1.5e+02f);
			float fThen183 = ((iTemp39) ? fElse177 : 1.8e+02f);
			float fElse183 = ((iTemp54) ? fElse182 : fThen182);
			float fThen184 = ((iTemp36) ? fElse176 : 1.2e+02f);
			float fElse184 = ((iTemp52) ? fElse183 : fThen183);
			float fElse185 = ((iTemp39) ? fElse184 : fThen184);
			float fElse186 = ((iTemp36) ? fElse185 : 1.2e+02f);
			float fElse187 = ((iTemp34) ? fElse186 : 1.2e+02f);
			float fElse188 = ((iTemp32) ? fElse187 : 1.2e+02f);
			float fThen189 = ((iTemp24) ? fElse175 : 1.3e+02f);
			float fElse189 = ((iTemp29) ? fElse188 : 1.2e+02f);
			float fThen190 = ((iTemp22) ? fElse174 : 1.2e+02f);
			float fElse190 = ((iTemp27) ? fElse189 : fThen189);
			float fElse191 = ((iTemp24) ? fElse190 : fThen190);
			float fElse192 = ((iTemp22) ? fElse191 : 1.2e+02f);
			float fElse193 = ((iTemp20) ? fElse192 : 1.2e+02f);
			float fThen194 = ((iTemp15) ? fElse173 : 1.3e+02f);
			float fElse194 = ((iTemp18) ? fElse193 : 1.2e+02f);
			float fThen195 = ((iTemp14) ? fElse172 : 1.5e+02f);
			float fElse195 = ((iTemp16) ? fElse194 : fThen194);
			float fThen196 = ((iTemp13) ? fElse171 : 1.2e+02f);
			float fElse196 = ((iTemp15) ? fElse195 : fThen195);
			float fElse197 = ((iTemp14) ? fElse196 : fThen196);
			float fThen198 = ((iTemp10) ? fElse170 : 1.8e+02f);
			float fElse198 = ((iTemp13) ? fElse197 : 1.2e+02f);
			float fThen199 = ((iTemp8) ? fElse169 : 1.3e+02f);
			float fElse199 = ((iTemp12) ? fElse198 : fThen198);
			float fThen200 = ((iTemp6) ? fElse168 : 1.2e+02f);
			float fElse200 = ((iTemp10) ? fElse199 : fThen199);
			float fElse201 = ((iTemp8) ? fElse200 : fThen200);
			float fThen202 = ((iTemp1) ? fElse167 : 1.3e+02f);
			float fElse202 = ((iTemp6) ? fElse201 : 1.2e+02f);
			float fThen203 = ((iTemp2) ? fElse166 : 1.2e+02f);
			float fElse203 = ((iTemp4) ? fElse202 : fThen202);
			float fTemp88 = ((iTemp1) ? fElse203 : fThen203) / fTemp77;
			float fTemp89 = 1.0f / fTemp78;
			float fTemp90 = (fTemp89 + fTemp88) / fTemp78 + 1.0f;
			float fTemp91 = fTemp78 * fTemp90;
			float fElse204 = 0.251189f - 0.051663f * fTemp3;
			float fElse205 = 0.151189f * fTemp5 + 0.1f;
			float fElse206 = 0.199526f - 0.099526f * fTemp7;
			float fElse207 = 0.398107f - 0.198581f * fTemp9;
			float fElse208 = 0.388107f * fTemp11 + 0.01f;
			float fElse209 = 0.079433f - 0.069433f * fTemp44;
			float fElse210 = 0.029314f * fTemp62 + 0.050119f;
			float fTemp92 = ((iTemp12) ? fElse210 : 0.079433f);
			float fElse211 = 0.040119f * fTemp45 + 0.01f;
			float fElse212 = 0.1f - 0.09f * fTemp47;
			float fElse213 = 0.068377f * fTemp17 + 0.031623f;
			float fElse214 = 0.079433f - 0.04781f * fTemp19;
			float fElse215 = 0.063584f * fTemp21 + 0.015849f;
			float fElse216 = 0.036904f * fTemp25 + 0.063096f;
			float fElse217 = 0.023285f * fTemp28 + 0.039811f;
			float fElse218 = 0.1f - 0.060189f * fTemp30;
			float fElse219 = 0.020567f * fTemp33 + 0.079433f;
			float fElse220 = 0.251189f - 0.171756f * fTemp35;
			float fElse221 = 0.354813f - 0.103624f * fTemp37;
			float fElse222 = 0.344813f * fTemp40 + 0.01f;
			float fElse223 = 0.039811f - 0.029811f * fTemp53;
			float fElse224 = 0.023962f * fTemp55 + 0.015849f;
			float fElse225 = 0.017783f - 0.001934f * fTemp76;
			float fElse226 = 0.001934f * fTemp0 + 0.015849f;
			float fThen227 = ((iTemp56) ? fElse226 : 0.017783f);
			float fThen228 = ((iTemp54) ? fElse225 : 0.015849f);
			float fElse228 = ((iTemp57) ? 0.015849f : fThen227);
			float fThen229 = ((iTemp52) ? fElse224 : 0.039811f);
			float fElse229 = ((iTemp56) ? fElse228 : fThen228);
			float fThen230 = ((iTemp39) ? fElse223 : 0.01f);
			float fElse230 = ((iTemp54) ? fElse229 : fThen229);
			float fThen231 = ((iTemp36) ? fElse222 : 0.354813f);
			float fElse231 = ((iTemp52) ? fElse230 : fThen230);
			float fThen232 = ((iTemp34) ? fElse221 : 0.251189f);
			float fElse232 = ((iTemp39) ? fElse231 : fThen231);
			float fThen233 = ((iTemp32) ? fElse220 : 0.079433f);
			float fElse233 = ((iTemp36) ? fElse232 : fThen232);
			float fThen234 = ((iTemp29) ? fElse219 : 0.1f);
			float fElse234 = ((iTemp34) ? fElse233 : fThen233);
			float fThen235 = ((iTemp27) ? fElse218 : 0.039811f);
			float fElse235 = ((iTemp32) ? fElse234 : fThen234);
			float fThen236 = ((iTemp24) ? fElse217 : 0.063096f);
			float fElse236 = ((iTemp29) ? fElse235 : fThen235);
			float fThen237 = ((iTemp22) ? fElse216 : 0.1f);
			float fElse237 = ((iTemp27) ? fElse236 : fThen236);
			float fElse238 = ((iTemp24) ? fElse237 : fThen237);
			float fThen239 = ((iTemp18) ? fElse215 : 0.079433f);
			float fElse239 = ((iTemp22) ? fElse238 : fTemp63);
			float fThen240 = ((iTemp16) ? fElse214 : 0.031623f);
			float fElse240 = ((iTemp20) ? fElse239 : fThen239);
			float fThen241 = ((iTemp15) ? fElse213 : 0.1f);
			float fElse241 = ((iTemp18) ? fElse240 : fThen240);
			float fThen242 = ((iTemp14) ? fElse212 : 0.01f);
			float fElse242 = ((iTemp16) ? fElse241 : fThen241);
			float fThen243 = ((iTemp13) ? fElse211 : 0.050119f);
			float fElse243 = ((iTemp15) ? fElse242 : fThen242);
			float fElse244 = ((iTemp14) ? fElse243 : fThen243);
			float fThen245 = ((iTemp10) ? fElse209 : 0.01f);
			float fElse245 = ((iTemp13) ? fElse244 : fTemp92);
			float fThen246 = ((iTemp8) ? fElse208 : 0.398107f);
			float fElse246 = ((iTemp12) ? fElse245 : fThen245);
			float fThen247 = ((iTemp6) ? fElse207 : 0.199526f);
			float fElse247 = ((iTemp10) ? fElse246 : fThen246);
			float fThen248 = ((iTemp4) ? fElse206 : 0.1f);
			float fElse248 = ((iTemp8) ? fElse247 : fThen247);
			float fThen249 = ((iTemp1) ? fElse205 : 0.251189f);
			float fElse249 = ((iTemp6) ? fElse248 : fThen248);
			float fThen250 = ((iTemp2) ? fElse204 : 0.199526f);
			float fElse250 = ((iTemp4) ? fElse249 : fThen249);
			float fTemp93 = ((iTemp1) ? fElse250 : fThen250);
			float fThen251 = fTemp67 * fTemp93;
			float fElse251 = fTemp68 * fTemp93;
			float fTemp94 = ((iSlow4) ? fElse251 : fThen251);
			fRec8[0] = fTemp74 - (fRec8[2] * ((fTemp89 - fTemp88) / fTemp78 + 1.0f) + 2.0f * fRec8[1] * (1.0f - 1.0f / mydsp_faustpower2_f(fTemp78))) / fTemp90;
			float fElse252 = fTemp75 + 2.6e+03f;
			float fElse253 = 2.8e+03f - 2e+02f * fTemp5;
			float fElse254 = 2e+02f * fTemp7 + 2.6e+03f;
			float fElse255 = 2.65e+03f - 5e+01f * fTemp9;
			float fElse256 = 2.7e+03f - fTemp81;
			float fElse257 = 2.83e+03f - 1.3e+02f * fTemp44;
			float fElse258 = 2.95e+03f - 1.2e+02f * fTemp62;
			float fElse259 = 1.5e+02f * fTemp45 + 2.8e+03f;
			float fElse260 = 2.9e+03f - 1e+02f * fTemp47;
			float fElse261 = 1.5e+02f * fTemp17 + 2.75e+03f;
			float fElse262 = 5e+01f * fTemp19 + 2.7e+03f;
			float fElse263 = 2.9e+03f - 2e+02f * fTemp21;
			float fElse264 = 2e+02f * fTemp23 + 2.7e+03f;
			float fElse265 = 2.75e+03f - fTemp26;
			float fElse266 = 3.5e+02f * fTemp28 + 2.4e+03f;
			float fElse267 = 2.6e+03f - 2e+02f * fTemp33;
			float fElse268 = 2e+02f * fTemp35 + 2.4e+03f;
			float fElse269 = 1.5e+02f * fTemp37 + 2.25e+03f;
			float fElse270 = 2.53e+03f - 2.8e+02f * fTemp40;
			float fElse271 = 2.83e+03f - 3e+02f * fTemp53;
			float fElse272 = 1.3e+02f * fTemp55 + 2.7e+03f;
			float fElse273 = 2.8e+03f - 1e+02f * fTemp0;
			float fThen274 = ((iTemp56) ? fElse273 : 2.7e+03f);
			float fElse275 = ((iTemp57) ? 2.8e+03f : fThen274);
			float fThen276 = ((iTemp52) ? fElse272 : 2.83e+03f);
			float fElse276 = ((iTemp56) ? fElse275 : 2.7e+03f);
			float fThen277 = ((iTemp39) ? fElse271 : 2.53e+03f);
			float fElse277 = ((iTemp54) ? fElse276 : fThen276);
			float fThen278 = ((iTemp36) ? fElse270 : 2.25e+03f);
			float fElse278 = ((iTemp52) ? fElse277 : fThen277);
			float fThen279 = ((iTemp34) ? fElse269 : 2.4e+03f);
			float fElse279 = ((iTemp39) ? fElse278 : fThen278);
			float fThen280 = ((iTemp32) ? fElse268 : 2.6e+03f);
			float fElse280 = ((iTemp36) ? fElse279 : fThen279);
			float fThen281 = ((iTemp29) ? fElse267 : 2.4e+03f);
			float fElse281 = ((iTemp34) ? fElse280 : fThen280);
			float fElse282 = ((iTemp32) ? fElse281 : fThen281);
			float fThen283 = ((iTemp24) ? fElse266 : 2.75e+03f);
			float fElse283 = ((iTemp29) ? fElse282 : 2.4e+03f);
			float fThen284 = ((iTemp22) ? fElse265 : 2.7e+03f);
			float fElse284 = ((iTemp27) ? fElse283 : fThen283);
			float fThen285 = ((iTemp20) ? fElse264 : 2.9e+03f);
			float fElse285 = ((iTemp24) ? fElse284 : fThen284);
			float fThen286 = ((iTemp18) ? fElse263 : 2.7e+03f);
			float fElse286 = ((iTemp22) ? fElse285 : fThen285);
			float fThen287 = ((iTemp16) ? fElse262 : 2.75e+03f);
			float fElse287 = ((iTemp20) ? fElse286 : fThen286);
			float fThen288 = ((iTemp15) ? fElse261 : 2.9e+03f);
			float fElse288 = ((iTemp18) ? fElse287 : fThen287);
			float fThen289 = ((iTemp14) ? fElse260 : 2.8e+03f);
			float fElse289 = ((iTemp16) ? fElse288 : fThen288);
			float fThen290 = ((iTemp13) ? fElse259 : 2.95e+03f);
			float fElse290 = ((iTemp15) ? fElse289 : fThen289);
			float fThen291 = ((iTemp12) ? fElse258 : 2.83e+03f);
			float fElse291 = ((iTemp14) ? fElse290 : fThen290);
			float fThen292 = ((iTemp10) ? fElse257 : 2.7e+03f);
			float fElse292 = ((iTemp13) ? fElse291 : fThen291);
			float fThen293 = ((iTemp8) ? fElse256 : 2.65e+03f);
			float fElse293 = ((iTemp12) ? fElse292 : fThen292);
			float fThen294 = ((iTemp6) ? fElse255 : 2.6e+03f);
			float fElse294 = ((iTemp10) ? fElse293 : fThen293);
			float fThen295 = ((iTemp4) ? fElse254 : 2.8e+03f);
			float fElse295 = ((iTemp8) ? fElse294 : fThen294);
			float fThen296 = ((iTemp1) ? fElse253 : 2.6e+03f);
			float fElse296 = ((iTemp6) ? fElse295 : fThen295);
			float fThen297 = ((iTemp2) ? fElse252 : 2.7e+03f);
			float fElse297 = ((iTemp4) ? fElse296 : fThen296);
			float fTemp95 = ((iTemp1) ? fElse297 : fThen297);
			float fTemp96 = std::tan(fConst3 * fTemp95);
			float fElse298 = 1.2e+02f - fTemp43;
			float fElse299 = 1.7e+02f - fTemp81;
			float fElse300 = 7e+01f * fTemp44 + 1e+02f;
			float fElse301 = 1.2e+02f - 2e+01f * fTemp45;
			float fElse302 = fTemp48 + 1e+02f;
			float fElse303 = 1.2e+02f - fTemp49;
			float fElse304 = fTemp50 + 1e+02f;
			float fElse305 = 1.1e+02f - fTemp51;
			float fElse306 = 1.7e+02f - fTemp85;
			float fElse307 = 7e+01f * fTemp53 + 1e+02f;
			float fElse308 = 1.2e+02f - fTemp86;
			float fThen309 = ((iTemp52) ? fElse308 : 1e+02f);
			float fThen310 = ((iTemp39) ? fElse307 : 1.7e+02f);
			float fElse310 = ((iTemp54) ? 1.2e+02f : fThen309);
			float fThen311 = ((iTemp36) ? fElse306 : 1.1e+02f);
			float fElse311 = ((iTemp52) ? fElse310 : fThen310);
			float fThen312 = ((iTemp34) ? fElse305 : 1e+02f);
			float fElse312 = ((iTemp39) ? fElse311 : fThen311);
			float fElse313 = ((iTemp36) ? fElse312 : fThen312);
			float fElse314 = ((iTemp34) ? fElse313 : 1e+02f);
			float fElse315 = ((iTemp32) ? fElse314 : 1e+02f);
			float fThen316 = ((iTemp24) ? fElse304 : 1.2e+02f);
			float fElse316 = ((iTemp29) ? fElse315 : 1e+02f);
			float fThen317 = ((iTemp22) ? fElse303 : 1e+02f);
			float fElse317 = ((iTemp27) ? fElse316 : fThen316);
			float fElse318 = ((iTemp24) ? fElse317 : fThen317);
			float fElse319 = ((iTemp22) ? fElse318 : 1e+02f);
			float fElse320 = ((iTemp20) ? fElse319 : 1e+02f);
			float fThen321 = ((iTemp15) ? fElse302 : 1.2e+02f);
			float fElse321 = ((iTemp18) ? fElse320 : 1e+02f);
			float fElse322 = ((iTemp16) ? fElse321 : fThen321);
			float fThen323 = ((iTemp13) ? fElse301 : 1e+02f);
			float fElse323 = ((iTemp15) ? fElse322 : 1.2e+02f);
			float fElse324 = ((iTemp14) ? fElse323 : fThen323);
			float fThen325 = ((iTemp10) ? fElse300 : 1.7e+02f);
			float fElse325 = ((iTemp13) ? fElse324 : 1e+02f);
			float fThen326 = ((iTemp8) ? fElse299 : 1.2e+02f);
			float fElse326 = ((iTemp12) ? fElse325 : fThen325);
			float fThen327 = ((iTemp6) ? fElse298 : 1e+02f);
			float fElse327 = ((iTemp10) ? fElse326 : fThen326);
			float fElse328 = ((iTemp8) ? fElse327 : fThen327);
			float fElse329 = ((iTemp6) ? fElse328 : 1e+02f);
			float fElse330 = ((iTemp4) ? fElse329 : 1e+02f);
			float fTemp97 = ((iTemp1) ? fElse330 : 1e+02f) / fTemp95;
			float fTemp98 = 1.0f / fTemp96;
			float fTemp99 = (fTemp98 + fTemp97) / fTemp96 + 1.0f;
			float fTemp100 = fTemp96 * fTemp99;
			float fElse331 = 0.251189f - 0.109935f * fTemp3;
			float fElse332 = 0.125296f * fTemp5 + 0.125893f;
			float fElse333 = 0.251189f - 0.125296f * fTemp7;
			float fElse334 = 0.446684f - 0.195495f * fTemp9;
			float fElse335 = 0.428901f * fTemp11 + 0.017783f;
			float fElse336 = 0.079433f - 0.06165f * fTemp44;
			float fElse337 = 0.177828f - 0.127709f * fTemp45;
			float fElse338 = 0.152709f * fTemp47 + 0.025119f;
			float fElse339 = 0.070795f - 0.045676f * fTemp17;
			float fElse340 = 0.020676f * fTemp19 + 0.050119f;
			float fElse341 = 0.063096f - 0.012977f * fTemp21;
			float fElse342 = 0.125893f - 0.062797f * fTemp23;
			float fElse343 = 0.055098f * fTemp25 + 0.070795f;
			float fElse344 = 0.045676f * fTemp28 + 0.025119f;
			float fElse345 = 0.089125f - 0.064006f * fTemp30;
			float fElse346 = 0.158489f - 0.069364f * fTemp33;
			float fElse347 = 0.354813f - 0.196324f * fTemp35;
			float fElse348 = 0.32319f * fTemp40 + 0.031623f;
			float fElse349 = 0.158489f - 0.126866f * fTemp53;
			float fElse350 = 0.126866f * fTemp55 + 0.031623f;
			float fElse351 = 0.1f - 0.068377f * fTemp0;
			float fThen352 = ((iTemp56) ? fElse351 : 0.031623f);
			float fElse353 = ((iTemp57) ? 0.1f : fThen352);
			float fThen354 = ((iTemp52) ? fElse350 : 0.158489f);
			float fElse354 = ((iTemp56) ? fElse353 : 0.031623f);
			float fThen355 = ((iTemp39) ? fElse349 : 0.031623f);
			float fElse355 = ((iTemp54) ? fElse354 : fThen354);
			float fThen356 = ((iTemp36) ? fElse348 : 0.354813f);
			float fElse356 = ((iTemp52) ? fElse355 : fThen355);
			float fElse357 = ((iTemp39) ? fElse356 : fThen356);
			float fThen358 = ((iTemp32) ? fElse347 : 0.158489f);
			float fElse358 = ((iTemp36) ? fElse357 : 0.354813f);
			float fThen359 = ((iTemp29) ? fElse346 : 0.089125f);
			float fElse359 = ((iTemp34) ? fElse358 : fThen358);
			float fThen360 = ((iTemp27) ? fElse345 : 0.025119f);
			float fElse360 = ((iTemp32) ? fElse359 : fThen359);
			float fThen361 = ((iTemp24) ? fElse344 : 0.070795f);
			float fElse361 = ((iTemp29) ? fElse360 : fThen360);
			float fThen362 = ((iTemp22) ? fElse343 : 0.125893f);
			float fElse362 = ((iTemp27) ? fElse361 : fThen361);
			float fThen363 = ((iTemp20) ? fElse342 : 0.063096f);
			float fElse363 = ((iTemp24) ? fElse362 : fThen362);
			float fThen364 = ((iTemp18) ? fElse341 : 0.050119f);
			float fElse364 = ((iTemp22) ? fElse363 : fThen363);
			float fThen365 = ((iTemp16) ? fElse340 : 0.070795f);
			float fElse365 = ((iTemp20) ? fElse364 : fThen364);
			float fThen366 = ((iTemp15) ? fElse339 : 0.025119f);
			float fElse366 = ((iTemp18) ? fElse365 : fThen365);
			float fThen367 = ((iTemp14) ? fElse338 : 0.177828f);
			float fElse367 = ((iTemp16) ? fElse366 : fThen366);
			float fThen368 = ((iTemp13) ? fElse337 : 0.050119f);
			float fElse368 = ((iTemp15) ? fElse367 : fThen367);
			float fElse369 = ((iTemp14) ? fElse368 : fThen368);
			float fThen370 = ((iTemp10) ? fElse336 : 0.017783f);
			float fElse370 = ((iTemp13) ? fElse369 : fTemp92);
			float fThen371 = ((iTemp8) ? fElse335 : 0.446684f);
			float fElse371 = ((iTemp12) ? fElse370 : fThen370);
			float fThen372 = ((iTemp6) ? fElse334 : 0.251189f);
			float fElse372 = ((iTemp10) ? fElse371 : fThen371);
			float fThen373 = ((iTemp4) ? fElse333 : 0.125893f);
			float fElse373 = ((iTemp8) ? fElse372 : fThen372);
			float fThen374 = ((iTemp1) ? fElse332 : 0.251189f);
			float fElse374 = ((iTemp6) ? fElse373 : fThen373);
			float fThen375 = ((iTemp2) ? fElse331 : 0.141254f);
			float fElse375 = ((iTemp4) ? fElse374 : fThen374);
			float fTemp101 = ((iTemp1) ? fElse375 : fThen375);
			float fThen376 = fTemp67 * fTemp101;
			float fElse376 = fTemp68 * fTemp101;
			float fTemp102 = ((iSlow4) ? fElse376 : fThen376);
			fRec9[0] = fTemp74 - (fRec9[2] * ((fTemp98 - fTemp97) / fTemp96 + 1.0f) + 2.0f * fRec9[1] * (1.0f - 1.0f / mydsp_faustpower2_f(fTemp96))) / fTemp99;
			float fElse377 = 8e+02f - 2e+02f * fTemp3;
			float fElse378 = 1.87e+03f - 1.07e+03f * fTemp5;
			float fElse379 = 1.7e+02f * fTemp7 + 1.7e+03f;
			float fElse380 = 6.2e+02f * fTemp9 + 1.08e+03f;
			float fElse381 = 3.8e+02f * fTemp11 + 7e+02f;
			float fElse382 = 8e+02f - 1e+02f * fTemp44;
			float fElse383 = 2.14e+03f - 1.34e+03f * fTemp62;
			float fElse384 = 1.4e+02f * fTemp45 + 2e+03f;
			float fElse385 = 8.5e+02f * fTemp47 + 1.15e+03f;
			float fElse386 = 5.2e+02f * fTemp17 + 6.3e+02f;
			float fElse387 = 8.2e+02f - 1.9e+02f * fTemp19;
			float fElse388 = 1.85e+03f - 1.03e+03f * fTemp21;
			float fElse389 = 5e+01f * fTemp23 + 1.8e+03f;
			float fElse390 = 6.8e+02f * fTemp25 + 1.12e+03f;
			float fElse391 = 5.2e+02f * fTemp28 + 6e+02f;
			float fElse392 = 7.5e+02f - 1.5e+02f * fTemp30;
			float fElse393 = 1.75e+03f - 1e+03f * fTemp33;
			float fElse394 = 1.3e+02f * fTemp35 + 1.62e+03f;
			float fElse395 = 5.8e+02f * fTemp37 + 1.04e+03f;
			float fElse396 = 3.4e+02f * fTemp40 + 7e+02f;
			float fElse397 = 8e+02f - 1e+02f * fTemp53;
			float fElse398 = 1.7e+03f - 9e+02f * fTemp55;
			float fElse399 = 1e+02f * fTemp76 + 1.6e+03f;
			float fElse400 = 4.5e+02f * fTemp0 + 1.15e+03f;
			float fThen401 = ((iTemp56) ? fElse400 : 1.6e+03f);
			float fThen402 = ((iTemp54) ? fElse399 : 1.7e+03f);
			float fElse402 = ((iTemp57) ? 1.15e+03f : fThen401);
			float fThen403 = ((iTemp52) ? fElse398 : 8e+02f);
			float fElse403 = ((iTemp56) ? fElse402 : fThen402);
			float fThen404 = ((iTemp39) ? fElse397 : 7e+02f);
			float fElse404 = ((iTemp54) ? fElse403 : fThen403);
			float fThen405 = ((iTemp36) ? fElse396 : 1.04e+03f);
			float fElse405 = ((iTemp52) ? fElse404 : fThen404);
			float fThen406 = ((iTemp34) ? fElse395 : 1.62e+03f);
			float fElse406 = ((iTemp39) ? fElse405 : fThen405);
			float fThen407 = ((iTemp32) ? fElse394 : 1.75e+03f);
			float fElse407 = ((iTemp36) ? fElse406 : fThen406);
			float fThen408 = ((iTemp29) ? fElse393 : 7.5e+02f);
			float fElse408 = ((iTemp34) ? fElse407 : fThen407);
			float fThen409 = ((iTemp27) ? fElse392 : 6e+02f);
			float fElse409 = ((iTemp32) ? fElse408 : fThen408);
			float fThen410 = ((iTemp24) ? fElse391 : 1.12e+03f);
			float fElse410 = ((iTemp29) ? fElse409 : fThen409);
			float fThen411 = ((iTemp22) ? fElse390 : 1.8e+03f);
			float fElse411 = ((iTemp27) ? fElse410 : fThen410);
			float fThen412 = ((iTemp20) ? fElse389 : 1.85e+03f);
			float fElse412 = ((iTemp24) ? fElse411 : fThen411);
			float fThen413 = ((iTemp18) ? fElse388 : 8.2e+02f);
			float fElse413 = ((iTemp22) ? fElse412 : fThen412);
			float fThen414 = ((iTemp16) ? fElse387 : 6.3e+02f);
			float fElse414 = ((iTemp20) ? fElse413 : fThen413);
			float fThen415 = ((iTemp15) ? fElse386 : 1.15e+03f);
			float fElse415 = ((iTemp18) ? fElse414 : fThen414);
			float fThen416 = ((iTemp14) ? fElse385 : 2e+03f);
			float fElse416 = ((iTemp16) ? fElse415 : fThen415);
			float fThen417 = ((iTemp13) ? fElse384 : 2.14e+03f);
			float fElse417 = ((iTemp15) ? fElse416 : fThen416);
			float fThen418 = ((iTemp12) ? fElse383 : 8e+02f);
			float fElse418 = ((iTemp14) ? fElse417 : fThen417);
			float fThen419 = ((iTemp10) ? fElse382 : 7e+02f);
			float fElse419 = ((iTemp13) ? fElse418 : fThen418);
			float fThen420 = ((iTemp8) ? fElse381 : 1.08e+03f);
			float fElse420 = ((iTemp12) ? fElse419 : fThen419);
			float fThen421 = ((iTemp6) ? fElse380 : 1.7e+03f);
			float fElse421 = ((iTemp10) ? fElse420 : fThen420);
			float fThen422 = ((iTemp4) ? fElse379 : 1.87e+03f);
			float fElse422 = ((iTemp8) ? fElse421 : fThen421);
			float fThen423 = ((iTemp1) ? fElse378 : 8e+02f);
			float fElse423 = ((iTemp6) ? fElse422 : fThen422);
			float fThen424 = ((iTemp2) ? fElse377 : 6e+02f);
			float fElse424 = ((iTemp4) ? fElse423 : fThen423);
			float fTemp103 = ((iTemp1) ? fElse424 : fThen424);
			float fTemp104 = fSlow11 * fTemp65 + 3e+01f;
			float fThen426 = ((fTemp103 <= fTemp104) ? fTemp104 : fTemp103);
			float fElse426 = fTemp103 - 0.00095238094f * (fTemp66 + -2e+02f) * (fTemp103 + -1.3e+03f);
			float fElse427 = (((fTemp103 >= 1.3e+03f) & (fTemp66 >= 2e+02f)) ? fElse426 : fThen426);
			float fTemp105 = ((iSlow10) ? fElse427 : fTemp103);
			float fTemp106 = std::tan(fConst3 * fTemp105);
			float fElse428 = 8e+01f - 2e+01f * fTemp3;
			float fElse429 = 9e+01f - fTemp79;
			float fElse430 = 1e+01f * fTemp7 + 8e+01f;
			float fElse431 = 9e+01f - fTemp80;
			float fElse432 = 3e+01f * fTemp11 + 6e+01f;
			float fElse433 = 8e+01f - 2e+01f * fTemp44;
			float fElse434 = 9e+01f - 1e+01f * fTemp62;
			float fElse435 = 1e+02f - 1e+01f * fTemp45;
			float fElse436 = 1e+01f * fTemp47 + 9e+01f;
			float fElse437 = 3e+01f * fTemp17 + 6e+01f;
			float fElse438 = 8e+01f - 2e+01f * fTemp19;
			float fElse439 = 9e+01f - 1e+01f * fTemp21;
			float fElse440 = 1e+01f * fTemp23 + 8e+01f;
			float fElse441 = 9e+01f - fTemp83;
			float fElse442 = fTemp84 + 8e+01f;
			float fElse443 = 9e+01f - 1e+01f * fTemp33;
			float fElse444 = 1e+01f * fTemp35 + 8e+01f;
			float fElse445 = fTemp51 + 7e+01f;
			float fTemp107 = 1e+01f * fTemp40;
			float fElse446 = fTemp107 + 6e+01f;
			float fTemp108 = 2e+01f * fTemp53;
			float fElse447 = 8e+01f - fTemp108;
			float fElse448 = 1e+02f - fTemp86;
			float fElse449 = 2e+01f * fTemp76 + 8e+01f;
			float fElse450 = 9e+01f - 1e+01f * fTemp0;
			float fThen451 = ((iTemp56) ? fElse450 : 8e+01f);
			float fThen452 = ((iTemp54) ? fElse449 : 1e+02f);
			float fElse452 = ((iTemp57) ? 9e+01f : fThen451);
			float fThen453 = ((iTemp52) ? fElse448 : 8e+01f);
			float fElse453 = ((iTemp56) ? fElse452 : fThen452);
			float fThen454 = ((iTemp39) ? fElse447 : 6e+01f);
			float fElse454 = ((iTemp54) ? fElse453 : fThen453);
			float fThen455 = ((iTemp36) ? fElse446 : 7e+01f);
			float fElse455 = ((iTemp52) ? fElse454 : fThen454);
			float fThen456 = ((iTemp34) ? fElse445 : 8e+01f);
			float fElse456 = ((iTemp39) ? fElse455 : fThen455);
			float fThen457 = ((iTemp32) ? fElse444 : 9e+01f);
			float fElse457 = ((iTemp36) ? fElse456 : fThen456);
			float fThen458 = ((iTemp29) ? fElse443 : 8e+01f);
			float fElse458 = ((iTemp34) ? fElse457 : fThen457);
			float fElse459 = ((iTemp32) ? fElse458 : fThen458);
			float fThen460 = ((iTemp24) ? fElse442 : 9e+01f);
			float fElse460 = ((iTemp29) ? fElse459 : 8e+01f);
			float fThen461 = ((iTemp22) ? fElse441 : 8e+01f);
			float fElse461 = ((iTemp27) ? fElse460 : fThen460);
			float fThen462 = ((iTemp20) ? fElse440 : 9e+01f);
			float fElse462 = ((iTemp24) ? fElse461 : fThen461);
			float fThen463 = ((iTemp18) ? fElse439 : 8e+01f);
			float fElse463 = ((iTemp22) ? fElse462 : fThen462);
			float fThen464 = ((iTemp16) ? fElse438 : 6e+01f);
			float fElse464 = ((iTemp20) ? fElse463 : fThen463);
			float fThen465 = ((iTemp15) ? fElse437 : 9e+01f);
			float fElse465 = ((iTemp18) ? fElse464 : fThen464);
			float fThen466 = ((iTemp14) ? fElse436 : 1e+02f);
			float fElse466 = ((iTemp16) ? fElse465 : fThen465);
			float fThen467 = ((iTemp13) ? fElse435 : 9e+01f);
			float fElse467 = ((iTemp15) ? fElse466 : fThen466);
			float fThen468 = ((iTemp12) ? fElse434 : 8e+01f);
			float fElse468 = ((iTemp14) ? fElse467 : fThen467);
			float fThen469 = ((iTemp10) ? fElse433 : 6e+01f);
			float fElse469 = ((iTemp13) ? fElse468 : fThen468);
			float fThen470 = ((iTemp8) ? fElse432 : 9e+01f);
			float fElse470 = ((iTemp12) ? fElse469 : fThen469);
			float fThen471 = ((iTemp6) ? fElse431 : 8e+01f);
			float fElse471 = ((iTemp10) ? fElse470 : fThen470);
			float fThen472 = ((iTemp4) ? fElse430 : 9e+01f);
			float fElse472 = ((iTemp8) ? fElse471 : fThen471);
			float fThen473 = ((iTemp1) ? fElse429 : 8e+01f);
			float fElse473 = ((iTemp6) ? fElse472 : fThen472);
			float fThen474 = ((iTemp2) ? fElse428 : 6e+01f);
			float fElse474 = ((iTemp4) ? fElse473 : fThen473);
			float fTemp109 = ((iTemp1) ? fElse474 : fThen474) / fTemp105;
			float fTemp110 = 1.0f / fTemp106;
			float fTemp111 = (fTemp110 + fTemp109) / fTemp106 + 1.0f;
			float fTemp112 = fTemp106 * fTemp111;
			float fElse475 = 0.316228f - 0.216228f * fTemp3;
			float fElse476 = 0.1384f * fTemp5 + 0.177828f;
			float fElse477 = 0.199526f - 0.021698f * fTemp7;
			float fElse478 = 0.501187f - 0.301661f * fTemp9;
			float fElse479 = 0.342698f * fTemp11 + 0.158489f;
			float fElse480 = 0.281838f - 0.123349f * fTemp44;
			float fElse481 = 0.030649f * fTemp62 + 0.251189f;
			float fElse482 = 0.151189f * fTemp45 + 0.1f;
			float fElse483 = 0.501187f - 0.401187f * fTemp47;
			float fElse484 = 0.401187f * fTemp17 + 0.1f;
			float fElse485 = 0.316228f - 0.216228f * fTemp19;
			float fElse486 = 0.253132f * fTemp21 + 0.063096f;
			float fElse487 = 0.199526f - 0.13643f * fTemp23;
			float fElse488 = 0.501187f - 0.301661f * fTemp25;
			float fElse489 = 0.401187f * fTemp28 + 0.1f;
			float fElse490 = 0.281838f - 0.181838f * fTemp30;
			float fElse491 = 0.250215f * fTemp33 + 0.031623f;
			float fElse492 = 0.251189f - 0.219566f * fTemp35;
			float fElse493 = 0.446684f - 0.195495f * fTemp37;
			float fElse494 = 0.195495f * fTemp40 + 0.251189f;
			float fElse495 = 0.354813f - 0.103624f * fTemp53;
			float fElse496 = 0.254813f * fTemp55 + 0.1f;
			float fElse497 = 0.036904f * fTemp76 + 0.063096f;
			float fElse498 = 0.630957f - 0.567861f * fTemp0;
			float fThen499 = ((iTemp56) ? fElse498 : 0.063096f);
			float fThen500 = ((iTemp54) ? fElse497 : 0.1f);
			float fElse500 = ((iTemp57) ? 0.630957f : fThen499);
			float fThen501 = ((iTemp52) ? fElse496 : 0.354813f);
			float fElse501 = ((iTemp56) ? fElse500 : fThen500);
			float fThen502 = ((iTemp39) ? fElse495 : 0.251189f);
			float fElse502 = ((iTemp54) ? fElse501 : fThen501);
			float fThen503 = ((iTemp36) ? fElse494 : 0.446684f);
			float fElse503 = ((iTemp52) ? fElse502 : fThen502);
			float fThen504 = ((iTemp34) ? fElse493 : 0.251189f);
			float fElse504 = ((iTemp39) ? fElse503 : fThen503);
			float fThen505 = ((iTemp32) ? fElse492 : 0.031623f);
			float fElse505 = ((iTemp36) ? fElse504 : fThen504);
			float fThen506 = ((iTemp29) ? fElse491 : 0.281838f);
			float fElse506 = ((iTemp34) ? fElse505 : fThen505);
			float fThen507 = ((iTemp27) ? fElse490 : 0.1f);
			float fElse507 = ((iTemp32) ? fElse506 : fThen506);
			float fThen508 = ((iTemp24) ? fElse489 : 0.501187f);
			float fElse508 = ((iTemp29) ? fElse507 : fThen507);
			float fThen509 = ((iTemp22) ? fElse488 : 0.199526f);
			float fElse509 = ((iTemp27) ? fElse508 : fThen508);
			float fThen510 = ((iTemp20) ? fElse487 : 0.063096f);
			float fElse510 = ((iTemp24) ? fElse509 : fThen509);
			float fThen511 = ((iTemp18) ? fElse486 : 0.316228f);
			float fElse511 = ((iTemp22) ? fElse510 : fThen510);
			float fThen512 = ((iTemp16) ? fElse485 : 0.1f);
			float fElse512 = ((iTemp20) ? fElse511 : fThen511);
			float fThen513 = ((iTemp15) ? fElse484 : 0.501187f);
			float fElse513 = ((iTemp18) ? fElse512 : fThen512);
			float fThen514 = ((iTemp14) ? fElse483 : 0.1f);
			float fElse514 = ((iTemp16) ? fElse513 : fThen513);
			float fThen515 = ((iTemp13) ? fElse482 : 0.251189f);
			float fElse515 = ((iTemp15) ? fElse514 : fThen514);
			float fThen516 = ((iTemp12) ? fElse481 : 0.281838f);
			float fElse516 = ((iTemp14) ? fElse515 : fThen515);
			float fThen517 = ((iTemp10) ? fElse480 : 0.158489f);
			float fElse517 = ((iTemp13) ? fElse516 : fThen516);
			float fThen518 = ((iTemp8) ? fElse479 : 0.501187f);
			float fElse518 = ((iTemp12) ? fElse517 : fThen517);
			float fThen519 = ((iTemp6) ? fElse478 : 0.199526f);
			float fElse519 = ((iTemp10) ? fElse518 : fThen518);
			float fThen520 = ((iTemp4) ? fElse477 : 0.177828f);
			float fElse520 = ((iTemp8) ? fElse519 : fThen519);
			float fThen521 = ((iTemp1) ? fElse476 : 0.316228f);
			float fElse521 = ((iTemp6) ? fElse520 : fThen520);
			float fThen522 = ((iTemp2) ? fElse475 : 0.1f);
			float fElse522 = ((iTemp4) ? fElse521 : fThen521);
			float fTemp113 = ((iTemp1) ? fElse522 : fThen522);
			float fThen523 = fTemp67 * fTemp113;
			float fElse523 = fTemp68 * fTemp113;
			float fTemp114 = ((iSlow4) ? fElse523 : fThen523);
			fRec10[0] = fTemp74 - (fRec10[2] * ((fTemp110 - fTemp109) / fTemp106 + 1.0f) + 2.0f * fRec10[1] * (1.0f - 1.0f / mydsp_faustpower2_f(fTemp106))) / fTemp111;
			float fElse524 = 4e+02f - 5e+01f * fTemp3;
			float fElse525 = 1.1e+02f * fTemp5 + 2.9e+02f;
			float fElse526 = 4e+02f - 1.1e+02f * fTemp7;
			float fElse527 = 6.5e+02f - 2.5e+02f * fTemp9;
			float fElse528 = 325.0f * (fTemp11 + 1.0f);
			float fElse529 = 4.5e+02f - 125.0f * fTemp44;
			float fElse530 = 1.8e+02f * fTemp62 + 2.7e+02f;
			float fElse531 = 3.5e+02f - fTemp46;
			float fElse532 = 8e+02f - 4.5e+02f * fTemp47;
			float fElse533 = 4.3e+02f * fTemp17 + 3.7e+02f;
			float fElse534 = 4.3e+02f - 6e+01f * fTemp19;
			float fElse535 = 1.6e+02f * fTemp21 + 2.7e+02f;
			float fElse536 = 4.4e+02f - 1.7e+02f * fTemp23;
			float fElse537 = 6.6e+02f - 2.2e+02f * fTemp25;
			float fElse538 = 3.1e+02f * fTemp28 + 3.5e+02f;
			float fElse539 = 4e+02f - fTemp31;
			float fElse540 = 1.5e+02f * fTemp33 + 2.5e+02f;
			float fElse541 = 4e+02f - 1.5e+02f * fTemp35;
			float fElse542 = 6e+02f - 2e+02f * fTemp37;
			float fElse543 = 275.0f * fTemp40 + 325.0f;
			float fElse544 = 4.5e+02f - 125.0f * fTemp53;
			float fElse545 = 1e+02f * fTemp55 + 3.5e+02f;
			float fElse546 = 4e+02f - 5e+01f * fTemp76;
			float fElse547 = 8e+02f - 4e+02f * fTemp0;
			float fThen548 = ((iTemp56) ? fElse547 : 4e+02f);
			float fThen549 = ((iTemp54) ? fElse546 : 3.5e+02f);
			float fElse549 = ((iTemp57) ? 8e+02f : fThen548);
			float fThen550 = ((iTemp52) ? fElse545 : 4.5e+02f);
			float fElse550 = ((iTemp56) ? fElse549 : fThen549);
			float fThen551 = ((iTemp39) ? fElse544 : 325.0f);
			float fElse551 = ((iTemp54) ? fElse550 : fThen550);
			float fThen552 = ((iTemp36) ? fElse543 : 6e+02f);
			float fElse552 = ((iTemp52) ? fElse551 : fThen551);
			float fThen553 = ((iTemp34) ? fElse542 : 4e+02f);
			float fElse553 = ((iTemp39) ? fElse552 : fThen552);
			float fThen554 = ((iTemp32) ? fElse541 : 2.5e+02f);
			float fElse554 = ((iTemp36) ? fElse553 : fThen553);
			float fThen555 = ((iTemp29) ? fElse540 : 4e+02f);
			float fElse555 = ((iTemp34) ? fElse554 : fThen554);
			float fThen556 = ((iTemp27) ? fElse539 : 3.5e+02f);
			float fElse556 = ((iTemp32) ? fElse555 : fThen555);
			float fThen557 = ((iTemp24) ? fElse538 : 6.6e+02f);
			float fElse557 = ((iTemp29) ? fElse556 : fThen556);
			float fThen558 = ((iTemp22) ? fElse537 : 4.4e+02f);
			float fElse558 = ((iTemp27) ? fElse557 : fThen557);
			float fThen559 = ((iTemp20) ? fElse536 : 2.7e+02f);
			float fElse559 = ((iTemp24) ? fElse558 : fThen558);
			float fThen560 = ((iTemp18) ? fElse535 : 4.3e+02f);
			float fElse560 = ((iTemp22) ? fElse559 : fThen559);
			float fThen561 = ((iTemp16) ? fElse534 : 3.7e+02f);
			float fElse561 = ((iTemp20) ? fElse560 : fThen560);
			float fThen562 = ((iTemp15) ? fElse533 : 8e+02f);
			float fElse562 = ((iTemp18) ? fElse561 : fThen561);
			float fThen563 = ((iTemp14) ? fElse532 : 3.5e+02f);
			float fElse563 = ((iTemp16) ? fElse562 : fThen562);
			float fThen564 = ((iTemp13) ? fElse531 : 2.7e+02f);
			float fElse564 = ((iTemp15) ? fElse563 : fThen563);
			float fThen565 = ((iTemp12) ? fElse530 : 4.5e+02f);
			float fElse565 = ((iTemp14) ? fElse564 : fThen564);
			float fThen566 = ((iTemp10) ? fElse529 : 325.0f);
			float fElse566 = ((iTemp13) ? fElse565 : fThen565);
			float fThen567 = ((iTemp8) ? fElse528 : 6.5e+02f);
			float fElse567 = ((iTemp12) ? fElse566 : fThen566);
			float fThen568 = ((iTemp6) ? fElse527 : 4e+02f);
			float fElse568 = ((iTemp10) ? fElse567 : fThen567);
			float fThen569 = ((iTemp4) ? fElse526 : 2.9e+02f);
			float fElse569 = ((iTemp8) ? fElse568 : fThen568);
			float fThen570 = ((iTemp1) ? fElse525 : 4e+02f);
			float fElse570 = ((iTemp6) ? fElse569 : fThen569);
			float fThen571 = ((iTemp2) ? fElse524 : 3.5e+02f);
			float fElse571 = ((iTemp4) ? fElse570 : fThen570);
			float fTemp115 = ((iTemp1) ? fElse571 : fThen571);
			float fTemp116 = ((fTemp115 <= fTemp66) ? fTemp66 : fTemp115);
			float fTemp117 = std::tan(fConst3 * fTemp116);
			float fElse573 = 7e+01f - 3e+01f * fTemp3;
			float fElse574 = 3e+01f * fTemp5 + 4e+01f;
			float fElse575 = 7e+01f - 3e+01f * fTemp7;
			float fElse576 = fTemp43 + 5e+01f;
			float fElse577 = 1e+01f * fTemp44 + 4e+01f;
			float fElse578 = 6e+01f - 2e+01f * fTemp62;
			float fElse579 = 8e+01f - fTemp82;
			float fElse580 = 4e+01f * (fTemp17 + 1.0f);
			float fElse581 = 7e+01f - 3e+01f * fTemp23;
			float fElse582 = 8e+01f - fTemp83;
			float fElse583 = 4e+01f * (fTemp28 + 1.0f);
			float fElse584 = 6e+01f - 2e+01f * fTemp33;
			float fElse585 = 2e+01f * fTemp35 + 4e+01f;
			float fElse586 = 6e+01f - 2e+01f * fTemp37;
			float fElse587 = fTemp107 + 5e+01f;
			float fElse588 = 7e+01f - fTemp108;
			float fElse589 = fTemp86 + 5e+01f;
			float fElse590 = 6e+01f - 1e+01f * fTemp76;
			float fElse591 = 8e+01f - fTemp87;
			float fThen592 = ((iTemp56) ? fElse591 : 6e+01f);
			float fThen593 = ((iTemp54) ? fElse590 : 5e+01f);
			float fElse593 = ((iTemp57) ? 8e+01f : fThen592);
			float fThen594 = ((iTemp52) ? fElse589 : 7e+01f);
			float fElse594 = ((iTemp56) ? fElse593 : fThen593);
			float fThen595 = ((iTemp39) ? fElse588 : 5e+01f);
			float fElse595 = ((iTemp54) ? fElse594 : fThen594);
			float fThen596 = ((iTemp36) ? fElse587 : 6e+01f);
			float fElse596 = ((iTemp52) ? fElse595 : fThen595);
			float fThen597 = ((iTemp34) ? fElse586 : 4e+01f);
			float fElse597 = ((iTemp39) ? fElse596 : fThen596);
			float fThen598 = ((iTemp32) ? fElse585 : 6e+01f);
			float fElse598 = ((iTemp36) ? fElse597 : fThen597);
			float fThen599 = ((iTemp29) ? fElse584 : 4e+01f);
			float fElse599 = ((iTemp34) ? fElse598 : fThen598);
			float fElse600 = ((iTemp32) ? fElse599 : fThen599);
			float fThen601 = ((iTemp24) ? fElse583 : 8e+01f);
			float fElse601 = ((iTemp29) ? fElse600 : 4e+01f);
			float fThen602 = ((iTemp22) ? fElse582 : 7e+01f);
			float fElse602 = ((iTemp27) ? fElse601 : fThen601);
			float fThen603 = ((iTemp20) ? fElse581 : 4e+01f);
			float fElse603 = ((iTemp24) ? fElse602 : fThen602);
			float fElse604 = ((iTemp22) ? fElse603 : fThen603);
			float fElse605 = ((iTemp20) ? fElse604 : 4e+01f);
			float fThen606 = ((iTemp15) ? fElse580 : 8e+01f);
			float fElse606 = ((iTemp18) ? fElse605 : 4e+01f);
			float fThen607 = ((iTemp14) ? fElse579 : 6e+01f);
			float fElse607 = ((iTemp16) ? fElse606 : fThen606);
			float fElse608 = ((iTemp15) ? fElse607 : fThen607);
			float fThen609 = ((iTemp12) ? fElse578 : 4e+01f);
			float fElse609 = ((iTemp14) ? fElse608 : 6e+01f);
			float fThen610 = ((iTemp10) ? fElse577 : 5e+01f);
			float fElse610 = ((iTemp13) ? fElse609 : fThen609);
			float fElse611 = ((iTemp12) ? fElse610 : fThen610);
			float fThen612 = ((iTemp6) ? fElse576 : 7e+01f);
			float fElse612 = ((iTemp10) ? fElse611 : 5e+01f);
			float fThen613 = ((iTemp4) ? fElse575 : 4e+01f);
			float fElse613 = ((iTemp8) ? fElse612 : fThen612);
			float fThen614 = ((iTemp1) ? fElse574 : 7e+01f);
			float fElse614 = ((iTemp6) ? fElse613 : fThen613);
			float fThen615 = ((iTemp2) ? fElse573 : 4e+01f);
			float fElse615 = ((iTemp4) ? fElse614 : fThen614);
			float fTemp118 = ((iTemp1) ? fElse615 : fThen615) / fTemp116;
			float fTemp119 = 1.0f / fTemp117;
			float fTemp120 = (fTemp119 + fTemp118) / fTemp117 + 1.0f;
			float fTemp121 = fTemp117 * fTemp120;
			float fTemp122 = ((iSlow4) ? fTemp68 : fTemp67);
			fRec11[0] = fTemp74 - (fRec11[2] * ((fTemp119 - fTemp118) / fTemp117 + 1.0f) + 2.0f * fRec11[1] * (1.0f - 1.0f / mydsp_faustpower2_f(fTemp117))) / fTemp120;
			fRec12[0] = fSlow12 + fConst2 * fRec12[1];
			float fTemp123 = fRec12[0] * (fRec3[2] * (0.0f - fTemp69 / fTemp61) + fRec8[2] * (0.0f - fTemp94 / fTemp91) + fRec9[2] * (0.0f - fTemp102 / fTemp100) + fRec10[2] * (0.0f - fTemp114 / fTemp112) + fRec11[0] * fTemp122 / fTemp121 + fRec11[2] * (0.0f - fTemp122 / fTemp121) + fRec10[0] * fTemp114 / fTemp112 + fRec9[0] * fTemp102 / fTemp100 + fRec8[0] * fTemp94 / fTemp91 + fRec3[0] * fTemp69 / fTemp61);
			output0[i0] = FAUSTFLOAT(fTemp123);
			output1[i0] = FAUSTFLOAT(fTemp123);
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			iRec4[1] = iRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
		}
	}

};

// END-FAUSTDSP

#endif
