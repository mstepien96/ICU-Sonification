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
	int iRec0[2];

public:

	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}

	void instanceInitmydspSIG0(int sample_rate) {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec0[l1] = 0;
		}
	}

	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec0[0] = (iVec0[1] + iRec0[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec0[0]));
			iVec0[1] = iVec0[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];

class mydsp : public dsp {

private:

	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	float fRec3[2];
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec2[2];
	float fVec1[2];
	float fConst2;
	float fConst3;
	FAUSTFLOAT fHslider3;
	float fRec4[2];
	FAUSTFLOAT fHslider4;
	float fRec1[2];
	float fConst4;
	float fConst5;
	float fRec5[2];
	float fRec6[2];
	float fRec7[3];
	float fRec8[2];
	float fRec9[2];
	float fRec10[2];
	float fRec11[3];
	float fRec12[2];
	float fRec13[2];
	float fRec14[2];
	float fRec15[3];
	float fRec16[2];
	float fRec17[2];
	float fRec18[2];
	float fRec19[3];
	float fRec20[2];
	float fRec21[2];
	float fRec22[2];
	float fRec23[3];
	FAUSTFLOAT fButton0;
	float fRec24[2];
	FAUSTFLOAT fHslider5;

public:

	void metadata(Meta* m) {
		m->declare("basics_lib_listInterp_author", "Romain Michon");
		m->declare("basics_lib_name", "Faust Basic Element Library");
		m->declare("basics_lib_version", "0.8");
		m->declare("compile_options", "-lang cpp -es 1 -mcd 16 -single -ftz 0");
		m->declare("copyright", "(c)Mike Olsen, CCRMA (Stanford University)");
		m->declare("description", "FOF vocal synthesizer.");
		m->declare("filename", "untitled.dsp");
		m->declare("filters_lib_fir_author", "Julius O. Smith III");
		m->declare("filters_lib_fir_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_fir_license", "MIT-style STK-4.3 license");
		m->declare("filters_lib_iir_author", "Julius O. Smith III");
		m->declare("filters_lib_iir_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_iir_license", "MIT-style STK-4.3 license");
		m->declare("filters_lib_lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_name", "Faust Filters Library");
		m->declare("filters_lib_tf2_author", "Julius O. Smith III");
		m->declare("filters_lib_tf2_copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters_lib_tf2_license", "MIT-style STK-4.3 license");
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
		m->declare("oscillators_lib_hs_oscsin_author", "Mike Olsen");
		m->declare("oscillators_lib_hs_phasor_author", "Mike Olsen, revised by Stéphane Letz");
		m->declare("oscillators_lib_lf_sawpos_author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators_lib_lf_sawpos_licence", "STK-4.3");
		m->declare("oscillators_lib_name", "Faust Oscillator Library");
		m->declare("oscillators_lib_version", "0.3");
		m->declare("physmodels_lib_SFFormantModelFofSmooth_author", "Mike Olsen");
		m->declare("physmodels_lib_SFFormantModelFofSmooth_ui_author", "Mike Olsen");
		m->declare("physmodels_lib_SFFormantModel_author", "Mike Olsen");
		m->declare("physmodels_lib_autobendFreq_author", "Mike Olsen");
		m->declare("physmodels_lib_fofSmooth_author", "Mike Olsen");
		m->declare("physmodels_lib_fof_author", "Mike Olsen");
		m->declare("physmodels_lib_formantFilterFofSmooth_author", "Mike Olsen");
		m->declare("physmodels_lib_formantFilterbankFofSmooth_author", "Mike Olsen");
		m->declare("physmodels_lib_formantFilterbank_author", "Mike Olsen");
		m->declare("physmodels_lib_name", "Faust Physical Models Library");
		m->declare("physmodels_lib_skirtWidthMultiplier_author", "Mike Olsen");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = std::exp(0.0f - 1e+03f / fConst0);
		fConst5 = 1.0f - fConst4;
	}

	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(6.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(4.4e+02f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.9f);
	}

	virtual void instanceClear() {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec5[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec10[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec16[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec17[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec18[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec20[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec21[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec22[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
			fRec23[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec24[l26] = 0.0f;
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
		ui_interface->declare(&fHslider2, "0", "");
		ui_interface->addHorizontalSlider("freq", &fHslider2, FAUSTFLOAT(4.4e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider5, "1", "");
		ui_interface->addHorizontalSlider("gain", &fHslider5, FAUSTFLOAT(0.9f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "2", "");
		ui_interface->addHorizontalSlider("voiceType", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider3, "3", "");
		ui_interface->addHorizontalSlider("vowel", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "5", "");
		ui_interface->addHorizontalSlider("vibratoFreq", &fHslider0, FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider1, "6", "");
		ui_interface->addHorizontalSlider("vibratoGain", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
		ui_interface->closeBox();
	}

	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = fConst1 * float(fHslider0);
		float fSlow1 = 0.1f * float(fHslider1);
		float fSlow2 = float(fHslider2);
		float fSlow3 = fConst1 * fSlow2;
		float fSlow4 = fConst2 * float(fHslider3);
		float fSlow5 = float(fHslider4);
		float fSlow6 = 5.0f * fSlow5;
		float fSlow7 = 5.0f * (1.0f - fSlow5);
		int iThen77 = ((fSlow5 == 3.0f) ? 1 : 0);
		int iSlow8 = ((fSlow5 == 0.0f) ? 1 : iThen77);
		int iSlow9 = iSlow8 >= 1;
		float fSlow10 = ((iSlow9) ? 174.61f : 82.41f);
		float fSlow11 = ((iSlow9) ? 1046.5f : 523.25f);
		float fSlow12 = fSlow11 - fSlow10;
		float fSlow13 = float(5 * iSlow8);
		int iSlow14 = iSlow8 == 0;
		int iSlow15 = fSlow5 != 2.0f;
		float fSlow16 = 2.0f * fSlow2;
		float fSlow17 = fConst2 * float(fButton0);
		float fSlow18 = 25.0f * float(fHslider5);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec3[0] = fSlow0 + (fRec3[1] - std::floor(fSlow0 + fRec3[1]));
			float fTemp0 = fSlow1 * ftbl0mydspSIG0[int(65536.0f * fRec3[0])] + 1.0f;
			float fTemp1 = fRec2[1] + fSlow3 * fTemp0;
			fRec2[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = float((fRec2[0] - fRec2[1]) < 0.0f);
			fVec1[0] = fTemp2;
			int iTemp3 = int(fTemp2);
			fRec4[0] = fSlow4 + fConst3 * fRec4[1];
			float fTemp4 = fSlow6 + fRec4[0];
			int iTemp5 = fTemp4 < 23.0f;
			int iTemp6 = fTemp4 < 24.0f;
			float fTemp7 = fSlow6 + fRec4[0] + -23.0f;
			float fElse0 = 3e+02f * fTemp7 + 3e+03f;
			int iTemp8 = fTemp4 < 22.0f;
			float fTemp9 = fSlow6 + fRec4[0] + -22.0f;
			float fElse1 = 3.54e+03f - 5.4e+02f * fTemp9;
			int iTemp10 = fTemp4 < 21.0f;
			float fTemp11 = fSlow6 + fRec4[0] + -21.0f;
			float fElse2 = 3.58e+03f - 4e+01f * fTemp11;
			int iTemp12 = fTemp4 < 2e+01f;
			float fTemp13 = fSlow6 + fRec4[0] + -2e+01f;
			float fElse3 = 3.3e+02f * fTemp13 + 3.25e+03f;
			int iTemp14 = fTemp4 < 19.0f;
			float fTemp15 = fSlow6 + fRec4[0] + -19.0f;
			float fElse4 = 4.95e+03f - 1.7e+03f * fTemp15;
			int iTemp16 = fTemp4 < 18.0f;
			int iTemp17 = fTemp4 < 17.0f;
			int iTemp18 = fTemp4 < 16.0f;
			int iTemp19 = fTemp4 < 15.0f;
			int iTemp20 = fTemp4 < 14.0f;
			float fTemp21 = fSlow6 + fRec4[0] + -14.0f;
			float fElse5 = 1.55e+03f * fTemp21 + 3.4e+03f;
			int iTemp22 = fTemp4 < 13.0f;
			float fTemp23 = fSlow6 + fRec4[0] + -13.0f;
			float fElse6 = 1e+02f * fTemp23 + 3.3e+03f;
			int iTemp24 = fTemp4 < 12.0f;
			float fTemp25 = fSlow6 + fRec4[0] + -12.0f;
			float fElse7 = 3.59e+03f - 2.9e+02f * fTemp25;
			int iTemp26 = fTemp4 < 11.0f;
			float fTemp27 = fSlow6 + fRec4[0] + -11.0f;
			float fElse8 = 2.9e+02f * fTemp27 + 3.3e+03f;
			int iTemp28 = fTemp4 < 1e+01f;
			float fTemp29 = fSlow6 + fRec4[0] + -1e+01f;
			float fTemp30 = 5e+01f * fTemp29;
			float fElse9 = 3.35e+03f - fTemp30;
			int iTemp31 = fTemp4 < 9.0f;
			float fTemp32 = fSlow6 + fRec4[0] + -9.0f;
			float fElse10 = 4e+02f * fTemp32 + 2.95e+03f;
			int iTemp33 = fTemp4 < 8.0f;
			float fTemp34 = fSlow6 + fRec4[0] + -8.0f;
			float fTemp35 = 5e+01f * fTemp34;
			float fElse11 = fTemp35 + 2.9e+03f;
			int iTemp36 = fTemp4 < 7.0f;
			float fTemp37 = fSlow6 + fRec4[0] + -7.0f;
			float fElse12 = 3.34e+03f - 4.4e+02f * fTemp37;
			int iTemp38 = fTemp4 < 6.0f;
			float fTemp39 = fSlow6 + fRec4[0] + -6.0f;
			float fElse13 = 2.4e+02f * fTemp39 + 3.1e+03f;
			int iTemp40 = fTemp4 < 5.0f;
			float fTemp41 = fRec4[0] - fSlow7;
			float fTemp42 = 3.5e+02f * fTemp41;
			float fElse14 = fTemp42 + 2.75e+03f;
			int iTemp43 = fTemp4 < 4.0f;
			float fTemp44 = fSlow6 + fRec4[0] + -4.0f;
			float fElse15 = 4.95e+03f - 2.2e+03f * fTemp44;
			float fThen16 = ((iTemp40) ? fElse15 : 2.75e+03f);
			float fThen17 = ((iTemp38) ? fElse14 : 3.1e+03f);
			float fElse17 = ((iTemp43) ? 4.95e+03f : fThen16);
			float fThen18 = ((iTemp36) ? fElse13 : 3.34e+03f);
			float fElse18 = ((iTemp40) ? fElse17 : fThen17);
			float fThen19 = ((iTemp33) ? fElse12 : 2.9e+03f);
			float fElse19 = ((iTemp38) ? fElse18 : fThen18);
			float fThen20 = ((iTemp31) ? fElse11 : 2.95e+03f);
			float fElse20 = ((iTemp36) ? fElse19 : fThen19);
			float fThen21 = ((iTemp28) ? fElse10 : 3.35e+03f);
			float fElse21 = ((iTemp33) ? fElse20 : fThen20);
			float fThen22 = ((iTemp26) ? fElse9 : 3.3e+03f);
			float fElse22 = ((iTemp31) ? fElse21 : fThen21);
			float fThen23 = ((iTemp24) ? fElse8 : 3.59e+03f);
			float fElse23 = ((iTemp28) ? fElse22 : fThen22);
			float fThen24 = ((iTemp22) ? fElse7 : 3.3e+03f);
			float fElse24 = ((iTemp26) ? fElse23 : fThen23);
			float fThen25 = ((iTemp20) ? fElse6 : 3.4e+03f);
			float fElse25 = ((iTemp24) ? fElse24 : fThen24);
			float fThen26 = ((iTemp19) ? fElse5 : 4.95e+03f);
			float fElse26 = ((iTemp22) ? fElse25 : fThen25);
			float fElse27 = ((iTemp20) ? fElse26 : fThen26);
			float fElse28 = ((iTemp19) ? fElse27 : 4.95e+03f);
			float fElse29 = ((iTemp18) ? fElse28 : 4.95e+03f);
			float fElse30 = ((iTemp17) ? fElse29 : 4.95e+03f);
			float fThen31 = ((iTemp12) ? fElse4 : 3.25e+03f);
			float fElse31 = ((iTemp16) ? fElse30 : 4.95e+03f);
			float fThen32 = ((iTemp10) ? fElse3 : 3.58e+03f);
			float fElse32 = ((iTemp14) ? fElse31 : fThen31);
			float fThen33 = ((iTemp8) ? fElse2 : 3.54e+03f);
			float fElse33 = ((iTemp12) ? fElse32 : fThen32);
			float fThen34 = ((iTemp5) ? fElse1 : 3e+03f);
			float fElse34 = ((iTemp10) ? fElse33 : fThen33);
			float fThen35 = ((iTemp6) ? fElse0 : 3.3e+03f);
			float fElse35 = ((iTemp8) ? fElse34 : fThen34);
			float fThen36 = fRec1[1] + fConst1 * ((iTemp5) ? fElse35 : fThen35);
			float fTemp45 = ((iTemp3) ? 0.0f : fThen36);
			fRec1[0] = fTemp45 - std::floor(fTemp45);
			float fElse37 = 135.0f - 15.0f * fTemp7;
			float fElse38 = 15.0f * fTemp9 + 1.2e+02f;
			float fTemp46 = 2e+01f * fTemp13;
			float fElse39 = 1.4e+02f - fTemp46;
			float fElse40 = 2e+02f - 6e+01f * fTemp15;
			float fTemp47 = fSlow6 + fRec4[0] + -18.0f;
			float fElse41 = 8e+01f * fTemp47 + 1.2e+02f;
			float fTemp48 = fSlow6 + fRec4[0] + -16.0f;
			float fTemp49 = 8e+01f * fTemp48;
			float fElse42 = 2e+02f - fTemp49;
			float fTemp50 = fSlow6 + fRec4[0] + -15.0f;
			float fElse43 = 6e+01f * fTemp50 + 1.4e+02f;
			float fTemp51 = 2e+01f * fTemp21;
			float fElse44 = fTemp51 + 1.2e+02f;
			float fTemp52 = 2e+01f * fTemp29;
			float fElse45 = 1.4e+02f - fTemp52;
			float fTemp53 = 2e+01f * fTemp32;
			float fElse46 = fTemp53 + 1.2e+02f;
			float fTemp54 = 1e+01f * fTemp41;
			float fElse47 = 1.3e+02f - fTemp54;
			float fElse48 = 2e+02f - 7e+01f * fTemp44;
			int iTemp55 = fTemp4 < 3.0f;
			float fTemp56 = fSlow6 + fRec4[0] + -3.0f;
			float fElse49 = 65.0f * fTemp56 + 135.0f;
			int iTemp57 = fTemp4 < 2.0f;
			float fTemp58 = fSlow6 + fRec4[0] + -2.0f;
			float fElse50 = 2e+02f - 65.0f * fTemp58;
			int iTemp59 = fTemp4 < 1.0f;
			int iTemp60 = fTemp4 < 0.0f;
			float fElse51 = 6e+01f * fTemp4 + 1.4e+02f;
			float fThen52 = ((iTemp59) ? fElse51 : 2e+02f);
			float fElse53 = ((iTemp60) ? 1.4e+02f : fThen52);
			float fThen54 = ((iTemp55) ? fElse50 : 135.0f);
			float fElse54 = ((iTemp59) ? fElse53 : 2e+02f);
			float fThen55 = ((iTemp43) ? fElse49 : 2e+02f);
			float fElse55 = ((iTemp57) ? fElse54 : fThen54);
			float fThen56 = ((iTemp40) ? fElse48 : 1.3e+02f);
			float fElse56 = ((iTemp55) ? fElse55 : fThen55);
			float fThen57 = ((iTemp38) ? fElse47 : 1.2e+02f);
			float fElse57 = ((iTemp43) ? fElse56 : fThen56);
			float fElse58 = ((iTemp40) ? fElse57 : fThen57);
			float fElse59 = ((iTemp38) ? fElse58 : 1.2e+02f);
			float fElse60 = ((iTemp36) ? fElse59 : 1.2e+02f);
			float fThen61 = ((iTemp28) ? fElse46 : 1.4e+02f);
			float fElse61 = ((iTemp33) ? fElse60 : 1.2e+02f);
			float fThen62 = ((iTemp26) ? fElse45 : 1.2e+02f);
			float fElse62 = ((iTemp31) ? fElse61 : fThen61);
			float fElse63 = ((iTemp28) ? fElse62 : fThen62);
			float fElse64 = ((iTemp26) ? fElse63 : 1.2e+02f);
			float fElse65 = ((iTemp24) ? fElse64 : 1.2e+02f);
			float fThen66 = ((iTemp19) ? fElse44 : 1.4e+02f);
			float fElse66 = ((iTemp22) ? fElse65 : 1.2e+02f);
			float fThen67 = ((iTemp18) ? fElse43 : 2e+02f);
			float fElse67 = ((iTemp20) ? fElse66 : fThen66);
			float fThen68 = ((iTemp17) ? fElse42 : 1.2e+02f);
			float fElse68 = ((iTemp19) ? fElse67 : fThen67);
			float fElse69 = ((iTemp18) ? fElse68 : fThen68);
			float fThen70 = ((iTemp14) ? fElse41 : 2e+02f);
			float fElse70 = ((iTemp17) ? fElse69 : 1.2e+02f);
			float fThen71 = ((iTemp12) ? fElse40 : 1.4e+02f);
			float fElse71 = ((iTemp16) ? fElse70 : fThen70);
			float fThen72 = ((iTemp10) ? fElse39 : 1.2e+02f);
			float fElse72 = ((iTemp14) ? fElse71 : fThen71);
			float fElse73 = ((iTemp12) ? fElse72 : fThen72);
			float fThen74 = ((iTemp5) ? fElse38 : 135.0f);
			float fElse74 = ((iTemp10) ? fElse73 : 1.2e+02f);
			float fThen75 = ((iTemp6) ? fElse37 : 1.2e+02f);
			float fElse75 = ((iTemp8) ? fElse74 : fThen74);
			float fTemp61 = ((iTemp5) ? fElse75 : fThen75);
			fRec5[0] = fConst5 * fTemp61 + fConst4 * fRec5[1];
			float fTemp62 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec5[0]));
			float fTemp63 = fSlow2 * fTemp0;
			float fThen80 = (fTemp63 - fSlow10) / fSlow12;
			float fThen81 = ((fTemp63 >= fSlow11) ? 1.0f : fThen80);
			float fTemp64 = fRec4[0] + fSlow13;
			int iTemp65 = fTemp64 >= 5.0f;
			int iTemp66 = fTemp64 >= 3.0f;
			int iTemp67 = fTemp64 >= 2.0f;
			int iTemp68 = fTemp64 >= 1.0f;
			float fThen83 = ((iTemp68) ? 1.25f : 1.0f);
			int iTemp69 = fTemp64 >= 4.0f;
			float fThen85 = ((iTemp67) ? 1.25f : fThen83);
			float fElse85 = ((iTemp69) ? 1.5f : 1.0f);
			int iTemp70 = fTemp64 >= 8.0f;
			int iTemp71 = fTemp64 >= 7.0f;
			int iTemp72 = fTemp64 >= 6.0f;
			float fThen87 = ((iTemp72) ? 3.0f : 2.0f);
			float fThen88 = ((iTemp71) ? 3.0f : fThen87);
			float fThen89 = ((iTemp66) ? fElse85 : fThen85);
			float fElse89 = ((iTemp70) ? 2.0f : fThen88);
			float fTemp73 = ((iTemp65) ? fElse89 : fThen89);
			float fThen91 = ((iTemp68) ? 2.5f : 1e+01f);
			float fThen93 = ((iTemp67) ? 2.5f : fThen91);
			float fElse93 = ((iTemp69) ? 4.0f : 1e+01f);
			float fThen95 = ((iTemp72) ? 12.0f : 15.0f);
			float fThen96 = ((iTemp71) ? 12.0f : fThen95);
			float fThen97 = ((iTemp66) ? fElse93 : fThen93);
			float fElse97 = ((iTemp70) ? 12.0f : fThen96);
			float fTemp74 = fTemp73 + (((iTemp65) ? fElse97 : fThen97) - fTemp73) * ((fTemp63 <= fSlow10) ? 0.0f : fThen81);
			fRec6[0] = fConst5 * fTemp74 * fTemp61 + fConst4 * fRec6[1];
			float fTemp75 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec6[0]));
			float fElse98 = 0.018496f * fTemp9 + 0.031623f;
			float fElse99 = 0.1f - 0.068377f * fTemp11;
			float fElse100 = 0.020567f * fTemp13 + 0.079433f;
			float fElse101 = 0.078433f * fTemp15 + 0.001f;
			float fElse102 = 0.003162f - 0.002162f * fTemp47;
			float fTemp76 = fSlow6 + fRec4[0] + -17.0f;
			float fElse103 = 0.00631f - 0.003148f * fTemp76;
			float fElse104 = 0.004725f * fTemp48 + 0.001585f;
			float fElse105 = 0.003162f - 0.001577f * fTemp50;
			float fElse106 = 0.019953f - 0.016791f * fTemp21;
			float fElse107 = 0.004104f * fTemp25 + 0.015849f;
			float fElse108 = 0.1f - 0.084151f * fTemp27;
			float fTemp77 = ((iTemp24) ? fElse108 : 0.015849f);
			float fElse109 = 0.087411f * fTemp29 + 0.012589f;
			float fElse110 = 0.015849f - 0.00326f * fTemp32;
			float fElse111 = 0.005849f * fTemp34 + 0.01f;
			float fElse112 = 0.039811f - 0.029811f * fTemp37;
			float fElse113 = 0.125893f - 0.086082f * fTemp39;
			float fElse114 = 0.025893f * fTemp41 + 0.1f;
			float fElse115 = 0.099369f * fTemp44 + 0.000631f;
			float fElse116 = 0.001778f - 0.001147f * fTemp56;
			float fElse117 = 0.000778f * fTemp58 + 0.001f;
			float fThen118 = ((iTemp55) ? fElse117 : 0.001778f);
			float fThen119 = ((iTemp43) ? fElse116 : 0.000631f);
			float fElse119 = ((iTemp57) ? 0.001f : fThen118);
			float fThen120 = ((iTemp40) ? fElse115 : 0.1f);
			float fElse120 = ((iTemp55) ? fElse119 : fThen119);
			float fThen121 = ((iTemp38) ? fElse114 : 0.125893f);
			float fElse121 = ((iTemp43) ? fElse120 : fThen120);
			float fThen122 = ((iTemp36) ? fElse113 : 0.039811f);
			float fElse122 = ((iTemp40) ? fElse121 : fThen121);
			float fThen123 = ((iTemp33) ? fElse112 : 0.01f);
			float fElse123 = ((iTemp38) ? fElse122 : fThen122);
			float fThen124 = ((iTemp31) ? fElse111 : 0.015849f);
			float fElse124 = ((iTemp36) ? fElse123 : fThen123);
			float fThen125 = ((iTemp28) ? fElse110 : 0.012589f);
			float fElse125 = ((iTemp33) ? fElse124 : fThen124);
			float fThen126 = ((iTemp26) ? fElse109 : 0.1f);
			float fElse126 = ((iTemp31) ? fElse125 : fThen125);
			float fElse127 = ((iTemp28) ? fElse126 : fThen126);
			float fThen128 = ((iTemp22) ? fElse107 : 0.019953f);
			float fElse128 = ((iTemp26) ? fElse127 : fTemp77);
			float fElse129 = ((iTemp24) ? fElse128 : fThen128);
			float fThen130 = ((iTemp19) ? fElse106 : 0.003162f);
			float fElse130 = ((iTemp22) ? fElse129 : 0.019953f);
			float fThen131 = ((iTemp18) ? fElse105 : 0.001585f);
			float fElse131 = ((iTemp20) ? fElse130 : fThen130);
			float fThen132 = ((iTemp17) ? fElse104 : 0.00631f);
			float fElse132 = ((iTemp19) ? fElse131 : fThen131);
			float fThen133 = ((iTemp16) ? fElse103 : 0.003162f);
			float fElse133 = ((iTemp18) ? fElse132 : fThen132);
			float fThen134 = ((iTemp14) ? fElse102 : 0.001f);
			float fElse134 = ((iTemp17) ? fElse133 : fThen133);
			float fThen135 = ((iTemp12) ? fElse101 : 0.079433f);
			float fElse135 = ((iTemp16) ? fElse134 : fThen134);
			float fThen136 = ((iTemp10) ? fElse100 : 0.1f);
			float fElse136 = ((iTemp14) ? fElse135 : fThen135);
			float fThen137 = ((iTemp8) ? fElse99 : 0.031623f);
			float fElse137 = ((iTemp12) ? fElse136 : fThen136);
			float fThen138 = ((iTemp5) ? fElse98 : 0.050119f);
			float fElse138 = ((iTemp10) ? fElse137 : fThen137);
			float fElse139 = ((iTemp8) ? fElse138 : fThen138);
			float fTemp78 = ((iTemp5) ? fElse139 : 0.050119f);
			float fTemp79 = 0.00084f * (1e+03f - fTemp63) + 0.8f;
			float fTemp80 = 0.0036666666f * (4e+02f - fTemp63) + 3.0f;
			float fThen140 = fTemp79 * fTemp78;
			float fElse140 = fTemp80 * fTemp78;
			fRec7[0] = fVec1[1] - (fRec7[1] * (0.0f - (fTemp75 + fTemp62)) + fTemp75 * fTemp62 * fRec7[2]);
			float fTemp81 = 1e+02f * fTemp7;
			float fElse141 = fTemp81 + 2.8e+03f;
			float fElse142 = 3.25e+03f - 4.5e+02f * fTemp9;
			float fElse143 = 5e+01f * fTemp11 + 3.2e+03f;
			float fElse144 = 3e+02f * fTemp13 + 2.9e+03f;
			float fElse145 = 3.8e+03f - 9e+02f * fTemp15;
			float fElse146 = 3.9e+03f - 1e+02f * fTemp76;
			float fElse147 = 3e+02f * fTemp48 + 3.6e+03f;
			float fElse148 = 3.9e+03f - 3e+02f * fTemp50;
			float fElse149 = 9e+02f * fTemp21 + 3e+03f;
			float fElse150 = 3.35e+03f - 3.5e+02f * fTemp25;
			float fElse151 = 3.5e+02f * fTemp27 + 3e+03f;
			float fElse152 = 325.0f * fTemp32 + 2675.0f;
			float fElse153 = 75.0f * fTemp34 + 2.6e+03f;
			float fElse154 = 3.05e+03f - 4.5e+02f * fTemp37;
			float fElse155 = 2.5e+02f * fTemp39 + 2.8e+03f;
			float fElse156 = fTemp42 + 2.45e+03f;
			float fElse157 = 3.5e+03f - 1.05e+03f * fTemp44;
			float fElse158 = 3.7e+03f - 2e+02f * fTemp58;
			float fTemp82 = fSlow6 + fRec4[0] + -1.0f;
			float fElse159 = 4e+02f * fTemp82 + 3.3e+03f;
			float fElse160 = 3.5e+03f - 2e+02f * fTemp4;
			float fThen161 = ((iTemp59) ? fElse160 : 3.3e+03f);
			float fThen162 = ((iTemp57) ? fElse159 : 3.7e+03f);
			float fElse162 = ((iTemp60) ? 3.5e+03f : fThen161);
			float fThen163 = ((iTemp55) ? fElse158 : 3.5e+03f);
			float fElse163 = ((iTemp59) ? fElse162 : fThen162);
			float fElse164 = ((iTemp57) ? fElse163 : fThen163);
			float fThen165 = ((iTemp40) ? fElse157 : 2.45e+03f);
			float fElse165 = ((iTemp55) ? fElse164 : 3.5e+03f);
			float fThen166 = ((iTemp38) ? fElse156 : 2.8e+03f);
			float fElse166 = ((iTemp43) ? fElse165 : fThen165);
			float fThen167 = ((iTemp36) ? fElse155 : 3.05e+03f);
			float fElse167 = ((iTemp40) ? fElse166 : fThen166);
			float fThen168 = ((iTemp33) ? fElse154 : 2.6e+03f);
			float fElse168 = ((iTemp38) ? fElse167 : fThen167);
			float fThen169 = ((iTemp31) ? fElse153 : 2675.0f);
			float fElse169 = ((iTemp36) ? fElse168 : fThen168);
			float fThen170 = ((iTemp28) ? fElse152 : 3e+03f);
			float fElse170 = ((iTemp33) ? fElse169 : fThen169);
			float fElse171 = ((iTemp31) ? fElse170 : fThen170);
			float fThen172 = ((iTemp24) ? fElse151 : 3.35e+03f);
			float fElse172 = ((iTemp28) ? fElse171 : 3e+03f);
			float fThen173 = ((iTemp22) ? fElse150 : 3e+03f);
			float fElse173 = ((iTemp26) ? fElse172 : fThen172);
			float fElse174 = ((iTemp24) ? fElse173 : fThen173);
			float fThen175 = ((iTemp19) ? fElse149 : 3.9e+03f);
			float fElse175 = ((iTemp22) ? fElse174 : 3e+03f);
			float fThen176 = ((iTemp18) ? fElse148 : 3.6e+03f);
			float fElse176 = ((iTemp20) ? fElse175 : fThen175);
			float fThen177 = ((iTemp17) ? fElse147 : 3.9e+03f);
			float fElse177 = ((iTemp19) ? fElse176 : fThen176);
			float fThen178 = ((iTemp16) ? fElse146 : 3.8e+03f);
			float fElse178 = ((iTemp18) ? fElse177 : fThen177);
			float fElse179 = ((iTemp17) ? fElse178 : fThen178);
			float fThen180 = ((iTemp12) ? fElse145 : 2.9e+03f);
			float fElse180 = ((iTemp16) ? fElse179 : 3.8e+03f);
			float fThen181 = ((iTemp10) ? fElse144 : 3.2e+03f);
			float fElse181 = ((iTemp14) ? fElse180 : fThen180);
			float fThen182 = ((iTemp8) ? fElse143 : 3.25e+03f);
			float fElse182 = ((iTemp12) ? fElse181 : fThen181);
			float fThen183 = ((iTemp5) ? fElse142 : 2.8e+03f);
			float fElse183 = ((iTemp10) ? fElse182 : fThen182);
			float fThen184 = ((iTemp6) ? fElse141 : 2.9e+03f);
			float fElse184 = ((iTemp8) ? fElse183 : fThen183);
			float fThen185 = fRec8[1] + fConst1 * ((iTemp5) ? fElse184 : fThen184);
			float fTemp83 = ((iTemp3) ? 0.0f : fThen185);
			fRec8[0] = fTemp83 - std::floor(fTemp83);
			float fElse186 = 1.3e+02f - 1e+01f * fTemp7;
			float fTemp84 = 1e+01f * fTemp9;
			float fElse187 = fTemp84 + 1.2e+02f;
			float fTemp85 = 1e+01f * fTemp13;
			float fElse188 = 1.3e+02f - fTemp85;
			float fTemp86 = 5e+01f * fTemp15;
			float fElse189 = 1.8e+02f - fTemp86;
			float fElse190 = 6e+01f * fTemp47 + 1.2e+02f;
			float fElse191 = 1.5e+02f - 3e+01f * fTemp48;
			float fTemp87 = 2e+01f * fTemp50;
			float fElse192 = fTemp87 + 1.3e+02f;
			float fElse193 = 1e+01f * fTemp21 + 1.2e+02f;
			float fTemp88 = 1e+01f * fTemp29;
			float fElse194 = 1.3e+02f - fTemp88;
			float fTemp89 = 1e+01f * fTemp32;
			float fElse195 = fTemp89 + 1.2e+02f;
			float fTemp90 = 6e+01f * fTemp44;
			float fElse196 = 1.8e+02f - fTemp90;
			float fElse197 = 5e+01f * fTemp56 + 1.3e+02f;
			float fTemp91 = 2e+01f * fTemp58;
			float fElse198 = 1.5e+02f - fTemp91;
			float fTemp92 = 2e+01f * fTemp4;
			float fElse199 = fTemp92 + 1.3e+02f;
			float fThen200 = ((iTemp59) ? fElse199 : 1.5e+02f);
			float fElse201 = ((iTemp60) ? 1.3e+02f : fThen200);
			float fThen202 = ((iTemp55) ? fElse198 : 1.3e+02f);
			float fElse202 = ((iTemp59) ? fElse201 : 1.5e+02f);
			float fThen203 = ((iTemp43) ? fElse197 : 1.8e+02f);
			float fElse203 = ((iTemp57) ? fElse202 : fThen202);
			float fThen204 = ((iTemp40) ? fElse196 : 1.2e+02f);
			float fElse204 = ((iTemp55) ? fElse203 : fThen203);
			float fElse205 = ((iTemp43) ? fElse204 : fThen204);
			float fElse206 = ((iTemp40) ? fElse205 : 1.2e+02f);
			float fElse207 = ((iTemp38) ? fElse206 : 1.2e+02f);
			float fElse208 = ((iTemp36) ? fElse207 : 1.2e+02f);
			float fThen209 = ((iTemp28) ? fElse195 : 1.3e+02f);
			float fElse209 = ((iTemp33) ? fElse208 : 1.2e+02f);
			float fThen210 = ((iTemp26) ? fElse194 : 1.2e+02f);
			float fElse210 = ((iTemp31) ? fElse209 : fThen209);
			float fElse211 = ((iTemp28) ? fElse210 : fThen210);
			float fElse212 = ((iTemp26) ? fElse211 : 1.2e+02f);
			float fElse213 = ((iTemp24) ? fElse212 : 1.2e+02f);
			float fThen214 = ((iTemp19) ? fElse193 : 1.3e+02f);
			float fElse214 = ((iTemp22) ? fElse213 : 1.2e+02f);
			float fThen215 = ((iTemp18) ? fElse192 : 1.5e+02f);
			float fElse215 = ((iTemp20) ? fElse214 : fThen214);
			float fThen216 = ((iTemp17) ? fElse191 : 1.2e+02f);
			float fElse216 = ((iTemp19) ? fElse215 : fThen215);
			float fElse217 = ((iTemp18) ? fElse216 : fThen216);
			float fThen218 = ((iTemp14) ? fElse190 : 1.8e+02f);
			float fElse218 = ((iTemp17) ? fElse217 : 1.2e+02f);
			float fThen219 = ((iTemp12) ? fElse189 : 1.3e+02f);
			float fElse219 = ((iTemp16) ? fElse218 : fThen218);
			float fThen220 = ((iTemp10) ? fElse188 : 1.2e+02f);
			float fElse220 = ((iTemp14) ? fElse219 : fThen219);
			float fElse221 = ((iTemp12) ? fElse220 : fThen220);
			float fThen222 = ((iTemp5) ? fElse187 : 1.3e+02f);
			float fElse222 = ((iTemp10) ? fElse221 : 1.2e+02f);
			float fThen223 = ((iTemp6) ? fElse186 : 1.2e+02f);
			float fElse223 = ((iTemp8) ? fElse222 : fThen222);
			float fTemp93 = ((iTemp5) ? fElse223 : fThen223);
			fRec9[0] = fConst5 * fTemp93 + fConst4 * fRec9[1];
			float fTemp94 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec9[0]));
			fRec10[0] = fConst5 * fTemp74 * fTemp93 + fConst4 * fRec10[1];
			float fTemp95 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec10[0]));
			float fElse224 = 0.251189f - 0.051663f * fTemp7;
			float fElse225 = 0.151189f * fTemp9 + 0.1f;
			float fElse226 = 0.199526f - 0.099526f * fTemp11;
			float fElse227 = 0.398107f - 0.198581f * fTemp13;
			float fElse228 = 0.388107f * fTemp15 + 0.01f;
			float fElse229 = 0.079433f - 0.069433f * fTemp47;
			float fElse230 = 0.029314f * fTemp76 + 0.050119f;
			float fTemp96 = ((iTemp16) ? fElse230 : 0.079433f);
			float fElse231 = 0.040119f * fTemp48 + 0.01f;
			float fElse232 = 0.1f - 0.09f * fTemp50;
			float fElse233 = 0.068377f * fTemp21 + 0.031623f;
			float fElse234 = 0.079433f - 0.04781f * fTemp23;
			float fElse235 = 0.063584f * fTemp25 + 0.015849f;
			float fElse236 = 0.036904f * fTemp29 + 0.063096f;
			float fElse237 = 0.023285f * fTemp32 + 0.039811f;
			float fElse238 = 0.1f - 0.060189f * fTemp34;
			float fElse239 = 0.020567f * fTemp37 + 0.079433f;
			float fElse240 = 0.251189f - 0.171756f * fTemp39;
			float fElse241 = 0.354813f - 0.103624f * fTemp41;
			float fElse242 = 0.344813f * fTemp44 + 0.01f;
			float fElse243 = 0.039811f - 0.029811f * fTemp56;
			float fElse244 = 0.023962f * fTemp58 + 0.015849f;
			float fElse245 = 0.017783f - 0.001934f * fTemp82;
			float fElse246 = 0.001934f * fTemp4 + 0.015849f;
			float fThen247 = ((iTemp59) ? fElse246 : 0.017783f);
			float fThen248 = ((iTemp57) ? fElse245 : 0.015849f);
			float fElse248 = ((iTemp60) ? 0.015849f : fThen247);
			float fThen249 = ((iTemp55) ? fElse244 : 0.039811f);
			float fElse249 = ((iTemp59) ? fElse248 : fThen248);
			float fThen250 = ((iTemp43) ? fElse243 : 0.01f);
			float fElse250 = ((iTemp57) ? fElse249 : fThen249);
			float fThen251 = ((iTemp40) ? fElse242 : 0.354813f);
			float fElse251 = ((iTemp55) ? fElse250 : fThen250);
			float fThen252 = ((iTemp38) ? fElse241 : 0.251189f);
			float fElse252 = ((iTemp43) ? fElse251 : fThen251);
			float fThen253 = ((iTemp36) ? fElse240 : 0.079433f);
			float fElse253 = ((iTemp40) ? fElse252 : fThen252);
			float fThen254 = ((iTemp33) ? fElse239 : 0.1f);
			float fElse254 = ((iTemp38) ? fElse253 : fThen253);
			float fThen255 = ((iTemp31) ? fElse238 : 0.039811f);
			float fElse255 = ((iTemp36) ? fElse254 : fThen254);
			float fThen256 = ((iTemp28) ? fElse237 : 0.063096f);
			float fElse256 = ((iTemp33) ? fElse255 : fThen255);
			float fThen257 = ((iTemp26) ? fElse236 : 0.1f);
			float fElse257 = ((iTemp31) ? fElse256 : fThen256);
			float fElse258 = ((iTemp28) ? fElse257 : fThen257);
			float fThen259 = ((iTemp22) ? fElse235 : 0.079433f);
			float fElse259 = ((iTemp26) ? fElse258 : fTemp77);
			float fThen260 = ((iTemp20) ? fElse234 : 0.031623f);
			float fElse260 = ((iTemp24) ? fElse259 : fThen259);
			float fThen261 = ((iTemp19) ? fElse233 : 0.1f);
			float fElse261 = ((iTemp22) ? fElse260 : fThen260);
			float fThen262 = ((iTemp18) ? fElse232 : 0.01f);
			float fElse262 = ((iTemp20) ? fElse261 : fThen261);
			float fThen263 = ((iTemp17) ? fElse231 : 0.050119f);
			float fElse263 = ((iTemp19) ? fElse262 : fThen262);
			float fElse264 = ((iTemp18) ? fElse263 : fThen263);
			float fThen265 = ((iTemp14) ? fElse229 : 0.01f);
			float fElse265 = ((iTemp17) ? fElse264 : fTemp96);
			float fThen266 = ((iTemp12) ? fElse228 : 0.398107f);
			float fElse266 = ((iTemp16) ? fElse265 : fThen265);
			float fThen267 = ((iTemp10) ? fElse227 : 0.199526f);
			float fElse267 = ((iTemp14) ? fElse266 : fThen266);
			float fThen268 = ((iTemp8) ? fElse226 : 0.1f);
			float fElse268 = ((iTemp12) ? fElse267 : fThen267);
			float fThen269 = ((iTemp5) ? fElse225 : 0.251189f);
			float fElse269 = ((iTemp10) ? fElse268 : fThen268);
			float fThen270 = ((iTemp6) ? fElse224 : 0.199526f);
			float fElse270 = ((iTemp8) ? fElse269 : fThen269);
			float fTemp97 = ((iTemp5) ? fElse270 : fThen270);
			float fThen271 = fTemp79 * fTemp97;
			float fElse271 = fTemp80 * fTemp97;
			fRec11[0] = fVec1[1] - (fRec11[1] * (0.0f - (fTemp95 + fTemp94)) + fTemp95 * fTemp94 * fRec11[2]);
			float fElse272 = fTemp81 + 2.6e+03f;
			float fElse273 = 2.8e+03f - 2e+02f * fTemp9;
			float fElse274 = 2e+02f * fTemp11 + 2.6e+03f;
			float fElse275 = 2.65e+03f - 5e+01f * fTemp13;
			float fElse276 = 2.7e+03f - fTemp86;
			float fElse277 = 2.83e+03f - 1.3e+02f * fTemp47;
			float fElse278 = 2.95e+03f - 1.2e+02f * fTemp76;
			float fElse279 = 1.5e+02f * fTemp48 + 2.8e+03f;
			float fElse280 = 2.9e+03f - 1e+02f * fTemp50;
			float fElse281 = 1.5e+02f * fTemp21 + 2.75e+03f;
			float fElse282 = 5e+01f * fTemp23 + 2.7e+03f;
			float fElse283 = 2.9e+03f - 2e+02f * fTemp25;
			float fElse284 = 2e+02f * fTemp27 + 2.7e+03f;
			float fElse285 = 2.75e+03f - fTemp30;
			float fElse286 = 3.5e+02f * fTemp32 + 2.4e+03f;
			float fElse287 = 2.6e+03f - 2e+02f * fTemp37;
			float fElse288 = 2e+02f * fTemp39 + 2.4e+03f;
			float fElse289 = 1.5e+02f * fTemp41 + 2.25e+03f;
			float fElse290 = 2.53e+03f - 2.8e+02f * fTemp44;
			float fElse291 = 2.83e+03f - 3e+02f * fTemp56;
			float fElse292 = 1.3e+02f * fTemp58 + 2.7e+03f;
			float fElse293 = 2.8e+03f - 1e+02f * fTemp4;
			float fThen294 = ((iTemp59) ? fElse293 : 2.7e+03f);
			float fElse295 = ((iTemp60) ? 2.8e+03f : fThen294);
			float fThen296 = ((iTemp55) ? fElse292 : 2.83e+03f);
			float fElse296 = ((iTemp59) ? fElse295 : 2.7e+03f);
			float fThen297 = ((iTemp43) ? fElse291 : 2.53e+03f);
			float fElse297 = ((iTemp57) ? fElse296 : fThen296);
			float fThen298 = ((iTemp40) ? fElse290 : 2.25e+03f);
			float fElse298 = ((iTemp55) ? fElse297 : fThen297);
			float fThen299 = ((iTemp38) ? fElse289 : 2.4e+03f);
			float fElse299 = ((iTemp43) ? fElse298 : fThen298);
			float fThen300 = ((iTemp36) ? fElse288 : 2.6e+03f);
			float fElse300 = ((iTemp40) ? fElse299 : fThen299);
			float fThen301 = ((iTemp33) ? fElse287 : 2.4e+03f);
			float fElse301 = ((iTemp38) ? fElse300 : fThen300);
			float fElse302 = ((iTemp36) ? fElse301 : fThen301);
			float fThen303 = ((iTemp28) ? fElse286 : 2.75e+03f);
			float fElse303 = ((iTemp33) ? fElse302 : 2.4e+03f);
			float fThen304 = ((iTemp26) ? fElse285 : 2.7e+03f);
			float fElse304 = ((iTemp31) ? fElse303 : fThen303);
			float fThen305 = ((iTemp24) ? fElse284 : 2.9e+03f);
			float fElse305 = ((iTemp28) ? fElse304 : fThen304);
			float fThen306 = ((iTemp22) ? fElse283 : 2.7e+03f);
			float fElse306 = ((iTemp26) ? fElse305 : fThen305);
			float fThen307 = ((iTemp20) ? fElse282 : 2.75e+03f);
			float fElse307 = ((iTemp24) ? fElse306 : fThen306);
			float fThen308 = ((iTemp19) ? fElse281 : 2.9e+03f);
			float fElse308 = ((iTemp22) ? fElse307 : fThen307);
			float fThen309 = ((iTemp18) ? fElse280 : 2.8e+03f);
			float fElse309 = ((iTemp20) ? fElse308 : fThen308);
			float fThen310 = ((iTemp17) ? fElse279 : 2.95e+03f);
			float fElse310 = ((iTemp19) ? fElse309 : fThen309);
			float fThen311 = ((iTemp16) ? fElse278 : 2.83e+03f);
			float fElse311 = ((iTemp18) ? fElse310 : fThen310);
			float fThen312 = ((iTemp14) ? fElse277 : 2.7e+03f);
			float fElse312 = ((iTemp17) ? fElse311 : fThen311);
			float fThen313 = ((iTemp12) ? fElse276 : 2.65e+03f);
			float fElse313 = ((iTemp16) ? fElse312 : fThen312);
			float fThen314 = ((iTemp10) ? fElse275 : 2.6e+03f);
			float fElse314 = ((iTemp14) ? fElse313 : fThen313);
			float fThen315 = ((iTemp8) ? fElse274 : 2.8e+03f);
			float fElse315 = ((iTemp12) ? fElse314 : fThen314);
			float fThen316 = ((iTemp5) ? fElse273 : 2.6e+03f);
			float fElse316 = ((iTemp10) ? fElse315 : fThen315);
			float fThen317 = ((iTemp6) ? fElse272 : 2.7e+03f);
			float fElse317 = ((iTemp8) ? fElse316 : fThen316);
			float fThen318 = fRec12[1] + fConst1 * ((iTemp5) ? fElse317 : fThen317);
			float fTemp98 = ((iTemp3) ? 0.0f : fThen318);
			fRec12[0] = fTemp98 - std::floor(fTemp98);
			float fElse319 = 1.2e+02f - fTemp46;
			float fElse320 = 1.7e+02f - fTemp86;
			float fElse321 = 7e+01f * fTemp47 + 1e+02f;
			float fElse322 = 1.2e+02f - 2e+01f * fTemp48;
			float fElse323 = fTemp51 + 1e+02f;
			float fElse324 = 1.2e+02f - fTemp52;
			float fElse325 = fTemp53 + 1e+02f;
			float fElse326 = 1.1e+02f - fTemp54;
			float fElse327 = 1.7e+02f - fTemp90;
			float fElse328 = 7e+01f * fTemp56 + 1e+02f;
			float fElse329 = 1.2e+02f - fTemp91;
			float fThen330 = ((iTemp55) ? fElse329 : 1e+02f);
			float fThen331 = ((iTemp43) ? fElse328 : 1.7e+02f);
			float fElse331 = ((iTemp57) ? 1.2e+02f : fThen330);
			float fThen332 = ((iTemp40) ? fElse327 : 1.1e+02f);
			float fElse332 = ((iTemp55) ? fElse331 : fThen331);
			float fThen333 = ((iTemp38) ? fElse326 : 1e+02f);
			float fElse333 = ((iTemp43) ? fElse332 : fThen332);
			float fElse334 = ((iTemp40) ? fElse333 : fThen333);
			float fElse335 = ((iTemp38) ? fElse334 : 1e+02f);
			float fElse336 = ((iTemp36) ? fElse335 : 1e+02f);
			float fThen337 = ((iTemp28) ? fElse325 : 1.2e+02f);
			float fElse337 = ((iTemp33) ? fElse336 : 1e+02f);
			float fThen338 = ((iTemp26) ? fElse324 : 1e+02f);
			float fElse338 = ((iTemp31) ? fElse337 : fThen337);
			float fElse339 = ((iTemp28) ? fElse338 : fThen338);
			float fElse340 = ((iTemp26) ? fElse339 : 1e+02f);
			float fElse341 = ((iTemp24) ? fElse340 : 1e+02f);
			float fThen342 = ((iTemp19) ? fElse323 : 1.2e+02f);
			float fElse342 = ((iTemp22) ? fElse341 : 1e+02f);
			float fElse343 = ((iTemp20) ? fElse342 : fThen342);
			float fThen344 = ((iTemp17) ? fElse322 : 1e+02f);
			float fElse344 = ((iTemp19) ? fElse343 : 1.2e+02f);
			float fElse345 = ((iTemp18) ? fElse344 : fThen344);
			float fThen346 = ((iTemp14) ? fElse321 : 1.7e+02f);
			float fElse346 = ((iTemp17) ? fElse345 : 1e+02f);
			float fThen347 = ((iTemp12) ? fElse320 : 1.2e+02f);
			float fElse347 = ((iTemp16) ? fElse346 : fThen346);
			float fThen348 = ((iTemp10) ? fElse319 : 1e+02f);
			float fElse348 = ((iTemp14) ? fElse347 : fThen347);
			float fElse349 = ((iTemp12) ? fElse348 : fThen348);
			float fElse350 = ((iTemp10) ? fElse349 : 1e+02f);
			float fElse351 = ((iTemp8) ? fElse350 : 1e+02f);
			float fTemp99 = ((iTemp5) ? fElse351 : 1e+02f);
			fRec13[0] = fConst5 * fTemp99 + fConst4 * fRec13[1];
			float fTemp100 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec13[0]));
			fRec14[0] = fConst5 * fTemp74 * fTemp99 + fConst4 * fRec14[1];
			float fTemp101 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec14[0]));
			float fElse352 = 0.251189f - 0.109935f * fTemp7;
			float fElse353 = 0.125296f * fTemp9 + 0.125893f;
			float fElse354 = 0.251189f - 0.125296f * fTemp11;
			float fElse355 = 0.446684f - 0.195495f * fTemp13;
			float fElse356 = 0.428901f * fTemp15 + 0.017783f;
			float fElse357 = 0.079433f - 0.06165f * fTemp47;
			float fElse358 = 0.177828f - 0.127709f * fTemp48;
			float fElse359 = 0.152709f * fTemp50 + 0.025119f;
			float fElse360 = 0.070795f - 0.045676f * fTemp21;
			float fElse361 = 0.020676f * fTemp23 + 0.050119f;
			float fElse362 = 0.063096f - 0.012977f * fTemp25;
			float fElse363 = 0.125893f - 0.062797f * fTemp27;
			float fElse364 = 0.055098f * fTemp29 + 0.070795f;
			float fElse365 = 0.045676f * fTemp32 + 0.025119f;
			float fElse366 = 0.089125f - 0.064006f * fTemp34;
			float fElse367 = 0.158489f - 0.069364f * fTemp37;
			float fElse368 = 0.354813f - 0.196324f * fTemp39;
			float fElse369 = 0.32319f * fTemp44 + 0.031623f;
			float fElse370 = 0.158489f - 0.126866f * fTemp56;
			float fElse371 = 0.126866f * fTemp58 + 0.031623f;
			float fElse372 = 0.1f - 0.068377f * fTemp4;
			float fThen373 = ((iTemp59) ? fElse372 : 0.031623f);
			float fElse374 = ((iTemp60) ? 0.1f : fThen373);
			float fThen375 = ((iTemp55) ? fElse371 : 0.158489f);
			float fElse375 = ((iTemp59) ? fElse374 : 0.031623f);
			float fThen376 = ((iTemp43) ? fElse370 : 0.031623f);
			float fElse376 = ((iTemp57) ? fElse375 : fThen375);
			float fThen377 = ((iTemp40) ? fElse369 : 0.354813f);
			float fElse377 = ((iTemp55) ? fElse376 : fThen376);
			float fElse378 = ((iTemp43) ? fElse377 : fThen377);
			float fThen379 = ((iTemp36) ? fElse368 : 0.158489f);
			float fElse379 = ((iTemp40) ? fElse378 : 0.354813f);
			float fThen380 = ((iTemp33) ? fElse367 : 0.089125f);
			float fElse380 = ((iTemp38) ? fElse379 : fThen379);
			float fThen381 = ((iTemp31) ? fElse366 : 0.025119f);
			float fElse381 = ((iTemp36) ? fElse380 : fThen380);
			float fThen382 = ((iTemp28) ? fElse365 : 0.070795f);
			float fElse382 = ((iTemp33) ? fElse381 : fThen381);
			float fThen383 = ((iTemp26) ? fElse364 : 0.125893f);
			float fElse383 = ((iTemp31) ? fElse382 : fThen382);
			float fThen384 = ((iTemp24) ? fElse363 : 0.063096f);
			float fElse384 = ((iTemp28) ? fElse383 : fThen383);
			float fThen385 = ((iTemp22) ? fElse362 : 0.050119f);
			float fElse385 = ((iTemp26) ? fElse384 : fThen384);
			float fThen386 = ((iTemp20) ? fElse361 : 0.070795f);
			float fElse386 = ((iTemp24) ? fElse385 : fThen385);
			float fThen387 = ((iTemp19) ? fElse360 : 0.025119f);
			float fElse387 = ((iTemp22) ? fElse386 : fThen386);
			float fThen388 = ((iTemp18) ? fElse359 : 0.177828f);
			float fElse388 = ((iTemp20) ? fElse387 : fThen387);
			float fThen389 = ((iTemp17) ? fElse358 : 0.050119f);
			float fElse389 = ((iTemp19) ? fElse388 : fThen388);
			float fElse390 = ((iTemp18) ? fElse389 : fThen389);
			float fThen391 = ((iTemp14) ? fElse357 : 0.017783f);
			float fElse391 = ((iTemp17) ? fElse390 : fTemp96);
			float fThen392 = ((iTemp12) ? fElse356 : 0.446684f);
			float fElse392 = ((iTemp16) ? fElse391 : fThen391);
			float fThen393 = ((iTemp10) ? fElse355 : 0.251189f);
			float fElse393 = ((iTemp14) ? fElse392 : fThen392);
			float fThen394 = ((iTemp8) ? fElse354 : 0.125893f);
			float fElse394 = ((iTemp12) ? fElse393 : fThen393);
			float fThen395 = ((iTemp5) ? fElse353 : 0.251189f);
			float fElse395 = ((iTemp10) ? fElse394 : fThen394);
			float fThen396 = ((iTemp6) ? fElse352 : 0.141254f);
			float fElse396 = ((iTemp8) ? fElse395 : fThen395);
			float fTemp102 = ((iTemp5) ? fElse396 : fThen396);
			float fThen397 = fTemp79 * fTemp102;
			float fElse397 = fTemp80 * fTemp102;
			fRec15[0] = fVec1[1] - (fRec15[1] * (0.0f - (fTemp101 + fTemp100)) + fTemp101 * fTemp100 * fRec15[2]);
			float fElse398 = 8e+02f - 2e+02f * fTemp7;
			float fElse399 = 1.87e+03f - 1.07e+03f * fTemp9;
			float fElse400 = 1.7e+02f * fTemp11 + 1.7e+03f;
			float fElse401 = 6.2e+02f * fTemp13 + 1.08e+03f;
			float fElse402 = 3.8e+02f * fTemp15 + 7e+02f;
			float fElse403 = 8e+02f - 1e+02f * fTemp47;
			float fElse404 = 2.14e+03f - 1.34e+03f * fTemp76;
			float fElse405 = 1.4e+02f * fTemp48 + 2e+03f;
			float fElse406 = 8.5e+02f * fTemp50 + 1.15e+03f;
			float fElse407 = 5.2e+02f * fTemp21 + 6.3e+02f;
			float fElse408 = 8.2e+02f - 1.9e+02f * fTemp23;
			float fElse409 = 1.85e+03f - 1.03e+03f * fTemp25;
			float fElse410 = 5e+01f * fTemp27 + 1.8e+03f;
			float fElse411 = 6.8e+02f * fTemp29 + 1.12e+03f;
			float fElse412 = 5.2e+02f * fTemp32 + 6e+02f;
			float fElse413 = 7.5e+02f - 1.5e+02f * fTemp34;
			float fElse414 = 1.75e+03f - 1e+03f * fTemp37;
			float fElse415 = 1.3e+02f * fTemp39 + 1.62e+03f;
			float fElse416 = 5.8e+02f * fTemp41 + 1.04e+03f;
			float fElse417 = 3.4e+02f * fTemp44 + 7e+02f;
			float fElse418 = 8e+02f - 1e+02f * fTemp56;
			float fElse419 = 1.7e+03f - 9e+02f * fTemp58;
			float fElse420 = 1e+02f * fTemp82 + 1.6e+03f;
			float fElse421 = 4.5e+02f * fTemp4 + 1.15e+03f;
			float fThen422 = ((iTemp59) ? fElse421 : 1.6e+03f);
			float fThen423 = ((iTemp57) ? fElse420 : 1.7e+03f);
			float fElse423 = ((iTemp60) ? 1.15e+03f : fThen422);
			float fThen424 = ((iTemp55) ? fElse419 : 8e+02f);
			float fElse424 = ((iTemp59) ? fElse423 : fThen423);
			float fThen425 = ((iTemp43) ? fElse418 : 7e+02f);
			float fElse425 = ((iTemp57) ? fElse424 : fThen424);
			float fThen426 = ((iTemp40) ? fElse417 : 1.04e+03f);
			float fElse426 = ((iTemp55) ? fElse425 : fThen425);
			float fThen427 = ((iTemp38) ? fElse416 : 1.62e+03f);
			float fElse427 = ((iTemp43) ? fElse426 : fThen426);
			float fThen428 = ((iTemp36) ? fElse415 : 1.75e+03f);
			float fElse428 = ((iTemp40) ? fElse427 : fThen427);
			float fThen429 = ((iTemp33) ? fElse414 : 7.5e+02f);
			float fElse429 = ((iTemp38) ? fElse428 : fThen428);
			float fThen430 = ((iTemp31) ? fElse413 : 6e+02f);
			float fElse430 = ((iTemp36) ? fElse429 : fThen429);
			float fThen431 = ((iTemp28) ? fElse412 : 1.12e+03f);
			float fElse431 = ((iTemp33) ? fElse430 : fThen430);
			float fThen432 = ((iTemp26) ? fElse411 : 1.8e+03f);
			float fElse432 = ((iTemp31) ? fElse431 : fThen431);
			float fThen433 = ((iTemp24) ? fElse410 : 1.85e+03f);
			float fElse433 = ((iTemp28) ? fElse432 : fThen432);
			float fThen434 = ((iTemp22) ? fElse409 : 8.2e+02f);
			float fElse434 = ((iTemp26) ? fElse433 : fThen433);
			float fThen435 = ((iTemp20) ? fElse408 : 6.3e+02f);
			float fElse435 = ((iTemp24) ? fElse434 : fThen434);
			float fThen436 = ((iTemp19) ? fElse407 : 1.15e+03f);
			float fElse436 = ((iTemp22) ? fElse435 : fThen435);
			float fThen437 = ((iTemp18) ? fElse406 : 2e+03f);
			float fElse437 = ((iTemp20) ? fElse436 : fThen436);
			float fThen438 = ((iTemp17) ? fElse405 : 2.14e+03f);
			float fElse438 = ((iTemp19) ? fElse437 : fThen437);
			float fThen439 = ((iTemp16) ? fElse404 : 8e+02f);
			float fElse439 = ((iTemp18) ? fElse438 : fThen438);
			float fThen440 = ((iTemp14) ? fElse403 : 7e+02f);
			float fElse440 = ((iTemp17) ? fElse439 : fThen439);
			float fThen441 = ((iTemp12) ? fElse402 : 1.08e+03f);
			float fElse441 = ((iTemp16) ? fElse440 : fThen440);
			float fThen442 = ((iTemp10) ? fElse401 : 1.7e+03f);
			float fElse442 = ((iTemp14) ? fElse441 : fThen441);
			float fThen443 = ((iTemp8) ? fElse400 : 1.87e+03f);
			float fElse443 = ((iTemp12) ? fElse442 : fThen442);
			float fThen444 = ((iTemp5) ? fElse399 : 8e+02f);
			float fElse444 = ((iTemp10) ? fElse443 : fThen443);
			float fThen445 = ((iTemp6) ? fElse398 : 6e+02f);
			float fElse445 = ((iTemp8) ? fElse444 : fThen444);
			float fTemp103 = ((iTemp5) ? fElse445 : fThen445);
			float fTemp104 = fSlow16 * fTemp0 + 3e+01f;
			float fThen447 = ((fTemp103 <= fTemp104) ? fTemp104 : fTemp103);
			float fElse447 = fTemp103 - 0.00095238094f * (fTemp63 + -2e+02f) * (fTemp103 + -1.3e+03f);
			float fElse448 = (((fTemp103 >= 1.3e+03f) & (fTemp63 >= 2e+02f)) ? fElse447 : fThen447);
			float fThen449 = fRec16[1] + fConst1 * ((iSlow15) ? fElse448 : fTemp103);
			float fTemp105 = ((iTemp3) ? 0.0f : fThen449);
			fRec16[0] = fTemp105 - std::floor(fTemp105);
			float fElse450 = 8e+01f - 2e+01f * fTemp7;
			float fElse451 = 9e+01f - fTemp84;
			float fElse452 = 1e+01f * fTemp11 + 8e+01f;
			float fElse453 = 9e+01f - fTemp85;
			float fElse454 = 3e+01f * fTemp15 + 6e+01f;
			float fElse455 = 8e+01f - 2e+01f * fTemp47;
			float fElse456 = 9e+01f - 1e+01f * fTemp76;
			float fElse457 = 1e+02f - 1e+01f * fTemp48;
			float fElse458 = 1e+01f * fTemp50 + 9e+01f;
			float fElse459 = 3e+01f * fTemp21 + 6e+01f;
			float fElse460 = 8e+01f - 2e+01f * fTemp23;
			float fElse461 = 9e+01f - 1e+01f * fTemp25;
			float fElse462 = 1e+01f * fTemp27 + 8e+01f;
			float fElse463 = 9e+01f - fTemp88;
			float fElse464 = fTemp89 + 8e+01f;
			float fElse465 = 9e+01f - 1e+01f * fTemp37;
			float fElse466 = 1e+01f * fTemp39 + 8e+01f;
			float fElse467 = fTemp54 + 7e+01f;
			float fTemp106 = 1e+01f * fTemp44;
			float fElse468 = fTemp106 + 6e+01f;
			float fTemp107 = 2e+01f * fTemp56;
			float fElse469 = 8e+01f - fTemp107;
			float fElse470 = 1e+02f - fTemp91;
			float fElse471 = 2e+01f * fTemp82 + 8e+01f;
			float fElse472 = 9e+01f - 1e+01f * fTemp4;
			float fThen473 = ((iTemp59) ? fElse472 : 8e+01f);
			float fThen474 = ((iTemp57) ? fElse471 : 1e+02f);
			float fElse474 = ((iTemp60) ? 9e+01f : fThen473);
			float fThen475 = ((iTemp55) ? fElse470 : 8e+01f);
			float fElse475 = ((iTemp59) ? fElse474 : fThen474);
			float fThen476 = ((iTemp43) ? fElse469 : 6e+01f);
			float fElse476 = ((iTemp57) ? fElse475 : fThen475);
			float fThen477 = ((iTemp40) ? fElse468 : 7e+01f);
			float fElse477 = ((iTemp55) ? fElse476 : fThen476);
			float fThen478 = ((iTemp38) ? fElse467 : 8e+01f);
			float fElse478 = ((iTemp43) ? fElse477 : fThen477);
			float fThen479 = ((iTemp36) ? fElse466 : 9e+01f);
			float fElse479 = ((iTemp40) ? fElse478 : fThen478);
			float fThen480 = ((iTemp33) ? fElse465 : 8e+01f);
			float fElse480 = ((iTemp38) ? fElse479 : fThen479);
			float fElse481 = ((iTemp36) ? fElse480 : fThen480);
			float fThen482 = ((iTemp28) ? fElse464 : 9e+01f);
			float fElse482 = ((iTemp33) ? fElse481 : 8e+01f);
			float fThen483 = ((iTemp26) ? fElse463 : 8e+01f);
			float fElse483 = ((iTemp31) ? fElse482 : fThen482);
			float fThen484 = ((iTemp24) ? fElse462 : 9e+01f);
			float fElse484 = ((iTemp28) ? fElse483 : fThen483);
			float fThen485 = ((iTemp22) ? fElse461 : 8e+01f);
			float fElse485 = ((iTemp26) ? fElse484 : fThen484);
			float fThen486 = ((iTemp20) ? fElse460 : 6e+01f);
			float fElse486 = ((iTemp24) ? fElse485 : fThen485);
			float fThen487 = ((iTemp19) ? fElse459 : 9e+01f);
			float fElse487 = ((iTemp22) ? fElse486 : fThen486);
			float fThen488 = ((iTemp18) ? fElse458 : 1e+02f);
			float fElse488 = ((iTemp20) ? fElse487 : fThen487);
			float fThen489 = ((iTemp17) ? fElse457 : 9e+01f);
			float fElse489 = ((iTemp19) ? fElse488 : fThen488);
			float fThen490 = ((iTemp16) ? fElse456 : 8e+01f);
			float fElse490 = ((iTemp18) ? fElse489 : fThen489);
			float fThen491 = ((iTemp14) ? fElse455 : 6e+01f);
			float fElse491 = ((iTemp17) ? fElse490 : fThen490);
			float fThen492 = ((iTemp12) ? fElse454 : 9e+01f);
			float fElse492 = ((iTemp16) ? fElse491 : fThen491);
			float fThen493 = ((iTemp10) ? fElse453 : 8e+01f);
			float fElse493 = ((iTemp14) ? fElse492 : fThen492);
			float fThen494 = ((iTemp8) ? fElse452 : 9e+01f);
			float fElse494 = ((iTemp12) ? fElse493 : fThen493);
			float fThen495 = ((iTemp5) ? fElse451 : 8e+01f);
			float fElse495 = ((iTemp10) ? fElse494 : fThen494);
			float fThen496 = ((iTemp6) ? fElse450 : 6e+01f);
			float fElse496 = ((iTemp8) ? fElse495 : fThen495);
			float fTemp108 = ((iTemp5) ? fElse496 : fThen496);
			fRec17[0] = fConst5 * fTemp108 + fConst4 * fRec17[1];
			float fTemp109 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec17[0]));
			fRec18[0] = fConst5 * fTemp74 * fTemp108 + fConst4 * fRec18[1];
			float fTemp110 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec18[0]));
			float fElse497 = 0.316228f - 0.216228f * fTemp7;
			float fElse498 = 0.1384f * fTemp9 + 0.177828f;
			float fElse499 = 0.199526f - 0.021698f * fTemp11;
			float fElse500 = 0.501187f - 0.301661f * fTemp13;
			float fElse501 = 0.342698f * fTemp15 + 0.158489f;
			float fElse502 = 0.281838f - 0.123349f * fTemp47;
			float fElse503 = 0.030649f * fTemp76 + 0.251189f;
			float fElse504 = 0.151189f * fTemp48 + 0.1f;
			float fElse505 = 0.501187f - 0.401187f * fTemp50;
			float fElse506 = 0.401187f * fTemp21 + 0.1f;
			float fElse507 = 0.316228f - 0.216228f * fTemp23;
			float fElse508 = 0.253132f * fTemp25 + 0.063096f;
			float fElse509 = 0.199526f - 0.13643f * fTemp27;
			float fElse510 = 0.501187f - 0.301661f * fTemp29;
			float fElse511 = 0.401187f * fTemp32 + 0.1f;
			float fElse512 = 0.281838f - 0.181838f * fTemp34;
			float fElse513 = 0.250215f * fTemp37 + 0.031623f;
			float fElse514 = 0.251189f - 0.219566f * fTemp39;
			float fElse515 = 0.446684f - 0.195495f * fTemp41;
			float fElse516 = 0.195495f * fTemp44 + 0.251189f;
			float fElse517 = 0.354813f - 0.103624f * fTemp56;
			float fElse518 = 0.254813f * fTemp58 + 0.1f;
			float fElse519 = 0.036904f * fTemp82 + 0.063096f;
			float fElse520 = 0.630957f - 0.567861f * fTemp4;
			float fThen521 = ((iTemp59) ? fElse520 : 0.063096f);
			float fThen522 = ((iTemp57) ? fElse519 : 0.1f);
			float fElse522 = ((iTemp60) ? 0.630957f : fThen521);
			float fThen523 = ((iTemp55) ? fElse518 : 0.354813f);
			float fElse523 = ((iTemp59) ? fElse522 : fThen522);
			float fThen524 = ((iTemp43) ? fElse517 : 0.251189f);
			float fElse524 = ((iTemp57) ? fElse523 : fThen523);
			float fThen525 = ((iTemp40) ? fElse516 : 0.446684f);
			float fElse525 = ((iTemp55) ? fElse524 : fThen524);
			float fThen526 = ((iTemp38) ? fElse515 : 0.251189f);
			float fElse526 = ((iTemp43) ? fElse525 : fThen525);
			float fThen527 = ((iTemp36) ? fElse514 : 0.031623f);
			float fElse527 = ((iTemp40) ? fElse526 : fThen526);
			float fThen528 = ((iTemp33) ? fElse513 : 0.281838f);
			float fElse528 = ((iTemp38) ? fElse527 : fThen527);
			float fThen529 = ((iTemp31) ? fElse512 : 0.1f);
			float fElse529 = ((iTemp36) ? fElse528 : fThen528);
			float fThen530 = ((iTemp28) ? fElse511 : 0.501187f);
			float fElse530 = ((iTemp33) ? fElse529 : fThen529);
			float fThen531 = ((iTemp26) ? fElse510 : 0.199526f);
			float fElse531 = ((iTemp31) ? fElse530 : fThen530);
			float fThen532 = ((iTemp24) ? fElse509 : 0.063096f);
			float fElse532 = ((iTemp28) ? fElse531 : fThen531);
			float fThen533 = ((iTemp22) ? fElse508 : 0.316228f);
			float fElse533 = ((iTemp26) ? fElse532 : fThen532);
			float fThen534 = ((iTemp20) ? fElse507 : 0.1f);
			float fElse534 = ((iTemp24) ? fElse533 : fThen533);
			float fThen535 = ((iTemp19) ? fElse506 : 0.501187f);
			float fElse535 = ((iTemp22) ? fElse534 : fThen534);
			float fThen536 = ((iTemp18) ? fElse505 : 0.1f);
			float fElse536 = ((iTemp20) ? fElse535 : fThen535);
			float fThen537 = ((iTemp17) ? fElse504 : 0.251189f);
			float fElse537 = ((iTemp19) ? fElse536 : fThen536);
			float fThen538 = ((iTemp16) ? fElse503 : 0.281838f);
			float fElse538 = ((iTemp18) ? fElse537 : fThen537);
			float fThen539 = ((iTemp14) ? fElse502 : 0.158489f);
			float fElse539 = ((iTemp17) ? fElse538 : fThen538);
			float fThen540 = ((iTemp12) ? fElse501 : 0.501187f);
			float fElse540 = ((iTemp16) ? fElse539 : fThen539);
			float fThen541 = ((iTemp10) ? fElse500 : 0.199526f);
			float fElse541 = ((iTemp14) ? fElse540 : fThen540);
			float fThen542 = ((iTemp8) ? fElse499 : 0.177828f);
			float fElse542 = ((iTemp12) ? fElse541 : fThen541);
			float fThen543 = ((iTemp5) ? fElse498 : 0.316228f);
			float fElse543 = ((iTemp10) ? fElse542 : fThen542);
			float fThen544 = ((iTemp6) ? fElse497 : 0.1f);
			float fElse544 = ((iTemp8) ? fElse543 : fThen543);
			float fTemp111 = ((iTemp5) ? fElse544 : fThen544);
			float fThen545 = fTemp79 * fTemp111;
			float fElse545 = fTemp80 * fTemp111;
			fRec19[0] = fVec1[1] - (fRec19[1] * (0.0f - (fTemp110 + fTemp109)) + fTemp110 * fTemp109 * fRec19[2]);
			float fElse546 = 4e+02f - 5e+01f * fTemp7;
			float fElse547 = 1.1e+02f * fTemp9 + 2.9e+02f;
			float fElse548 = 4e+02f - 1.1e+02f * fTemp11;
			float fElse549 = 6.5e+02f - 2.5e+02f * fTemp13;
			float fElse550 = 325.0f * (fTemp15 + 1.0f);
			float fElse551 = 4.5e+02f - 125.0f * fTemp47;
			float fElse552 = 1.8e+02f * fTemp76 + 2.7e+02f;
			float fElse553 = 3.5e+02f - fTemp49;
			float fElse554 = 8e+02f - 4.5e+02f * fTemp50;
			float fElse555 = 4.3e+02f * fTemp21 + 3.7e+02f;
			float fElse556 = 4.3e+02f - 6e+01f * fTemp23;
			float fElse557 = 1.6e+02f * fTemp25 + 2.7e+02f;
			float fElse558 = 4.4e+02f - 1.7e+02f * fTemp27;
			float fElse559 = 6.6e+02f - 2.2e+02f * fTemp29;
			float fElse560 = 3.1e+02f * fTemp32 + 3.5e+02f;
			float fElse561 = 4e+02f - fTemp35;
			float fElse562 = 1.5e+02f * fTemp37 + 2.5e+02f;
			float fElse563 = 4e+02f - 1.5e+02f * fTemp39;
			float fElse564 = 6e+02f - 2e+02f * fTemp41;
			float fElse565 = 275.0f * fTemp44 + 325.0f;
			float fElse566 = 4.5e+02f - 125.0f * fTemp56;
			float fElse567 = 1e+02f * fTemp58 + 3.5e+02f;
			float fElse568 = 4e+02f - 5e+01f * fTemp82;
			float fElse569 = 8e+02f - 4e+02f * fTemp4;
			float fThen570 = ((iTemp59) ? fElse569 : 4e+02f);
			float fThen571 = ((iTemp57) ? fElse568 : 3.5e+02f);
			float fElse571 = ((iTemp60) ? 8e+02f : fThen570);
			float fThen572 = ((iTemp55) ? fElse567 : 4.5e+02f);
			float fElse572 = ((iTemp59) ? fElse571 : fThen571);
			float fThen573 = ((iTemp43) ? fElse566 : 325.0f);
			float fElse573 = ((iTemp57) ? fElse572 : fThen572);
			float fThen574 = ((iTemp40) ? fElse565 : 6e+02f);
			float fElse574 = ((iTemp55) ? fElse573 : fThen573);
			float fThen575 = ((iTemp38) ? fElse564 : 4e+02f);
			float fElse575 = ((iTemp43) ? fElse574 : fThen574);
			float fThen576 = ((iTemp36) ? fElse563 : 2.5e+02f);
			float fElse576 = ((iTemp40) ? fElse575 : fThen575);
			float fThen577 = ((iTemp33) ? fElse562 : 4e+02f);
			float fElse577 = ((iTemp38) ? fElse576 : fThen576);
			float fThen578 = ((iTemp31) ? fElse561 : 3.5e+02f);
			float fElse578 = ((iTemp36) ? fElse577 : fThen577);
			float fThen579 = ((iTemp28) ? fElse560 : 6.6e+02f);
			float fElse579 = ((iTemp33) ? fElse578 : fThen578);
			float fThen580 = ((iTemp26) ? fElse559 : 4.4e+02f);
			float fElse580 = ((iTemp31) ? fElse579 : fThen579);
			float fThen581 = ((iTemp24) ? fElse558 : 2.7e+02f);
			float fElse581 = ((iTemp28) ? fElse580 : fThen580);
			float fThen582 = ((iTemp22) ? fElse557 : 4.3e+02f);
			float fElse582 = ((iTemp26) ? fElse581 : fThen581);
			float fThen583 = ((iTemp20) ? fElse556 : 3.7e+02f);
			float fElse583 = ((iTemp24) ? fElse582 : fThen582);
			float fThen584 = ((iTemp19) ? fElse555 : 8e+02f);
			float fElse584 = ((iTemp22) ? fElse583 : fThen583);
			float fThen585 = ((iTemp18) ? fElse554 : 3.5e+02f);
			float fElse585 = ((iTemp20) ? fElse584 : fThen584);
			float fThen586 = ((iTemp17) ? fElse553 : 2.7e+02f);
			float fElse586 = ((iTemp19) ? fElse585 : fThen585);
			float fThen587 = ((iTemp16) ? fElse552 : 4.5e+02f);
			float fElse587 = ((iTemp18) ? fElse586 : fThen586);
			float fThen588 = ((iTemp14) ? fElse551 : 325.0f);
			float fElse588 = ((iTemp17) ? fElse587 : fThen587);
			float fThen589 = ((iTemp12) ? fElse550 : 6.5e+02f);
			float fElse589 = ((iTemp16) ? fElse588 : fThen588);
			float fThen590 = ((iTemp10) ? fElse549 : 4e+02f);
			float fElse590 = ((iTemp14) ? fElse589 : fThen589);
			float fThen591 = ((iTemp8) ? fElse548 : 2.9e+02f);
			float fElse591 = ((iTemp12) ? fElse590 : fThen590);
			float fThen592 = ((iTemp5) ? fElse547 : 4e+02f);
			float fElse592 = ((iTemp10) ? fElse591 : fThen591);
			float fThen593 = ((iTemp6) ? fElse546 : 3.5e+02f);
			float fElse593 = ((iTemp8) ? fElse592 : fThen592);
			float fTemp112 = ((iTemp5) ? fElse593 : fThen593);
			float fThen595 = fRec20[1] + fConst1 * ((fTemp112 <= fTemp63) ? fTemp63 : fTemp112);
			float fTemp113 = ((iTemp3) ? 0.0f : fThen595);
			fRec20[0] = fTemp113 - std::floor(fTemp113);
			float fElse596 = 7e+01f - 3e+01f * fTemp7;
			float fElse597 = 3e+01f * fTemp9 + 4e+01f;
			float fElse598 = 7e+01f - 3e+01f * fTemp11;
			float fElse599 = fTemp46 + 5e+01f;
			float fElse600 = 1e+01f * fTemp47 + 4e+01f;
			float fElse601 = 6e+01f - 2e+01f * fTemp76;
			float fElse602 = 8e+01f - fTemp87;
			float fElse603 = 4e+01f * (fTemp21 + 1.0f);
			float fElse604 = 7e+01f - 3e+01f * fTemp27;
			float fElse605 = 8e+01f - fTemp88;
			float fElse606 = 4e+01f * (fTemp32 + 1.0f);
			float fElse607 = 6e+01f - 2e+01f * fTemp37;
			float fElse608 = 2e+01f * fTemp39 + 4e+01f;
			float fElse609 = 6e+01f - 2e+01f * fTemp41;
			float fElse610 = fTemp106 + 5e+01f;
			float fElse611 = 7e+01f - fTemp107;
			float fElse612 = fTemp91 + 5e+01f;
			float fElse613 = 6e+01f - 1e+01f * fTemp82;
			float fElse614 = 8e+01f - fTemp92;
			float fThen615 = ((iTemp59) ? fElse614 : 6e+01f);
			float fThen616 = ((iTemp57) ? fElse613 : 5e+01f);
			float fElse616 = ((iTemp60) ? 8e+01f : fThen615);
			float fThen617 = ((iTemp55) ? fElse612 : 7e+01f);
			float fElse617 = ((iTemp59) ? fElse616 : fThen616);
			float fThen618 = ((iTemp43) ? fElse611 : 5e+01f);
			float fElse618 = ((iTemp57) ? fElse617 : fThen617);
			float fThen619 = ((iTemp40) ? fElse610 : 6e+01f);
			float fElse619 = ((iTemp55) ? fElse618 : fThen618);
			float fThen620 = ((iTemp38) ? fElse609 : 4e+01f);
			float fElse620 = ((iTemp43) ? fElse619 : fThen619);
			float fThen621 = ((iTemp36) ? fElse608 : 6e+01f);
			float fElse621 = ((iTemp40) ? fElse620 : fThen620);
			float fThen622 = ((iTemp33) ? fElse607 : 4e+01f);
			float fElse622 = ((iTemp38) ? fElse621 : fThen621);
			float fElse623 = ((iTemp36) ? fElse622 : fThen622);
			float fThen624 = ((iTemp28) ? fElse606 : 8e+01f);
			float fElse624 = ((iTemp33) ? fElse623 : 4e+01f);
			float fThen625 = ((iTemp26) ? fElse605 : 7e+01f);
			float fElse625 = ((iTemp31) ? fElse624 : fThen624);
			float fThen626 = ((iTemp24) ? fElse604 : 4e+01f);
			float fElse626 = ((iTemp28) ? fElse625 : fThen625);
			float fElse627 = ((iTemp26) ? fElse626 : fThen626);
			float fElse628 = ((iTemp24) ? fElse627 : 4e+01f);
			float fThen629 = ((iTemp19) ? fElse603 : 8e+01f);
			float fElse629 = ((iTemp22) ? fElse628 : 4e+01f);
			float fThen630 = ((iTemp18) ? fElse602 : 6e+01f);
			float fElse630 = ((iTemp20) ? fElse629 : fThen629);
			float fElse631 = ((iTemp19) ? fElse630 : fThen630);
			float fThen632 = ((iTemp16) ? fElse601 : 4e+01f);
			float fElse632 = ((iTemp18) ? fElse631 : 6e+01f);
			float fThen633 = ((iTemp14) ? fElse600 : 5e+01f);
			float fElse633 = ((iTemp17) ? fElse632 : fThen632);
			float fElse634 = ((iTemp16) ? fElse633 : fThen633);
			float fThen635 = ((iTemp10) ? fElse599 : 7e+01f);
			float fElse635 = ((iTemp14) ? fElse634 : 5e+01f);
			float fThen636 = ((iTemp8) ? fElse598 : 4e+01f);
			float fElse636 = ((iTemp12) ? fElse635 : fThen635);
			float fThen637 = ((iTemp5) ? fElse597 : 7e+01f);
			float fElse637 = ((iTemp10) ? fElse636 : fThen636);
			float fThen638 = ((iTemp6) ? fElse596 : 4e+01f);
			float fElse638 = ((iTemp8) ? fElse637 : fThen637);
			float fTemp114 = ((iTemp5) ? fElse638 : fThen638);
			fRec21[0] = fConst5 * fTemp114 + fConst4 * fRec21[1];
			float fTemp115 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec21[0]));
			fRec22[0] = fConst5 * fTemp74 * fTemp114 + fConst4 * fRec22[1];
			float fTemp116 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec22[0]));
			fRec23[0] = fVec1[1] - (fRec23[1] * (0.0f - (fTemp116 + fTemp115)) + fTemp116 * fTemp115 * fRec23[2]);
			fRec24[0] = fSlow17 + fConst3 * fRec24[1];
			float fTemp117 = fSlow18 * fRec24[0] * (fRec23[0] * ((iSlow14) ? fTemp80 : fTemp79) * (1.0f - (fTemp115 + fTemp116 * (1.0f - fTemp115))) * ftbl0mydspSIG0[int(65536.0f * fRec20[0])] + fRec19[0] * ((iSlow14) ? fElse545 : fThen545) * (1.0f - (fTemp109 + fTemp110 * (1.0f - fTemp109))) * ftbl0mydspSIG0[int(65536.0f * fRec16[0])] + fRec15[0] * ((iSlow14) ? fElse397 : fThen397) * (1.0f - (fTemp100 + fTemp101 * (1.0f - fTemp100))) * ftbl0mydspSIG0[int(65536.0f * fRec12[0])] + fRec11[0] * ((iSlow14) ? fElse271 : fThen271) * (1.0f - (fTemp94 + fTemp95 * (1.0f - fTemp94))) * ftbl0mydspSIG0[int(65536.0f * fRec8[0])] + fRec7[0] * ((iSlow14) ? fElse140 : fThen140) * (1.0f - (fTemp62 + fTemp75 * (1.0f - fTemp62))) * ftbl0mydspSIG0[int(65536.0f * fRec1[0])]);
			output0[i0] = FAUSTFLOAT(fTemp117);
			output1[i0] = FAUSTFLOAT(fTemp117);
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fVec1[1] = fVec1[0];
			fRec4[1] = fRec4[0];
			fRec1[1] = fRec1[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
		}
	}

};

// END-FAUSTDSP

#endif
