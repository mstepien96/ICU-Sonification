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


class mydsp : public dsp {

private:

    FAUSTFLOAT fHslider0;
    int fSampleRate;
    float fConst0;
    float fRec0[2];

public:

    void metadata(Meta* m) {
        m->declare("compile_options", "-single -scal -I libraries/ -I project/ -lang wasm");
        m->declare("filename", "untitled3.dsp");
        m->declare("library_path0", "/libraries/stdfaust.lib");
        m->declare("library_path1", "/libraries/maths.lib");
        m->declare("library_path2", "/libraries/platform.lib");
        m->declare("maths_lib_author", "GRAME");
        m->declare("maths_lib_copyright", "GRAME");
        m->declare("maths_lib_license", "LGPL with exception");
        m->declare("maths_lib_name", "Faust Math Library");
        m->declare("maths_lib_version", "2.5");
        m->declare("name", "untitled3");
        m->declare("platform_lib_name", "Generic Platform Library");
        m->declare("platform_lib_version", "0.2");
        m->declare("version", "2.52.3");
    }

    virtual int getNumInputs() {
        return 0;
    }
    virtual int getNumOutputs() {
        return 2;
    }

    static void classInit(int sample_rate) {
    }

    virtual void instanceConstants(int sample_rate) {
        fSampleRate = sample_rate;
        fConst0 = 1.0f / std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
    }

    virtual void instanceResetUserInterface() {
        fHslider0 = FAUSTFLOAT(1e+02f);
    }

    virtual void instanceClear() {
        for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
            fRec0[l0] = 0.0f;
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
        ui_interface->openVerticalBox("untitled3");
        ui_interface->addHorizontalSlider("freq", &fHslider0, FAUSTFLOAT(1e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(2e+03f), FAUSTFLOAT(0.01f));
        ui_interface->closeBox();
    }

    virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
        FAUSTFLOAT* output0 = outputs[0];
        FAUSTFLOAT* output1 = outputs[1];
        float fSlow0 = fConst0 * float(fHslider0);
        for (int i0 = 0; i0 < count; i0 = i0 + 1) {
            fRec0[0] = fSlow0 + (fRec0[1] - std::floor(fSlow0 + fRec0[1]));
            float fTemp0 = std::sin(6.2831855f * fRec0[0]);
            output0[i0] = FAUSTFLOAT(fTemp0);
            output1[i0] = FAUSTFLOAT(fTemp0);
            fRec0[1] = fRec0[0];
        }
    }

};

// END-FAUSTDSP

#endif
