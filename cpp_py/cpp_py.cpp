// cpp_py.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include "pch.h"
#include <iostream>

int sample(/*int argc, char *argv[]*/)
{
	int argc = 5;
	char* argv[5];
	char c0[] = "cpp_py";
	char c1[] = "sample";
	char c2[] = "multiply";
	char c3[] = "5";
	char c4[] = "5";
	argv[0] = c0;
	argv[1] = c1;
	argv[2] = c2;
	argv[3] = c3;
	argv[4] = c4;

	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;
	int i;

	if (argc < 3) {
		fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
		return 1;
	}

	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault(argv[1]);
	/* Error checking of pName left out */

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, argv[2]);
		/* pFunc is a new reference */

		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(argc - 3);
			for (i = 0; i < argc - 3; ++i) {
				pValue = PyLong_FromLong(atoi(argv[i + 3]));
				if (!pValue) {
					Py_DECREF(pArgs);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return 1;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem(pArgs, i, pValue);
			}
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL) {
				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr, "Call failed\n");
				return 1;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}
	if (Py_FinalizeEx() < 0) {
		return 120;
	}
	return 0;
}
void test()
{
	PyObject *pValue, *pName, *pModu, *pFunc, *pArgs;


	Py_Initialize();

	char c0[] = "aaa.testmodule";
	pName = PyUnicode_DecodeFSDefault(c0);

	pModu = PyImport_Import(pName);

	Py_DECREF(pName);

	char c1[] = "thiking";
	pFunc = PyObject_GetAttrString(pModu, c1);

	long l = 100;
	pValue = PyLong_FromLong(l);

	int i = 1;
	pArgs = PyTuple_New(i);

	PyTuple_SetItem(pArgs, 0, pValue);


	pValue = PyObject_CallObject(pFunc, pArgs);

	int r = PyFloat_AsDouble(pValue);

	Py_DECREF(pValue);

	Py_Finalize();

	int t = r & 0b101;
	printf("%d", r);
}

static int numargs = 0;

static PyObject* emb_numargs(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return PyLong_FromLong(numargs);
}

static PyMethodDef EmbMethods[] = {
	{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
	{NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {
	PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,	NULL, NULL, NULL, NULL
};

static PyObject* PyInit_emb(void)
{
	return PyModule_Create(&EmbModule);
}

void ts()
{
	PyObject *pName, *pModule, *pFunc,*pArgs ,*pValue;

	numargs = 6;
	PyImport_AppendInittab("emb", &PyInit_emb);

	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault("testmodule");
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	pFunc = PyObject_GetAttrString(pModule, "tess");
	pArgs = PyTuple_New(0);
	pValue = PyObject_CallObject(pFunc,pArgs);
	Py_DECREF(pModule);
	Py_DECREF(pFunc);
	Py_DECREF(pValue);
}


//Simulation *sim = new Simulation;

void simts()
{

}

std::vector<ThreadTest*> tts;
void threexe()
{
	for (int i = 0; i < 32; i++)
	{
		ThreadTest* tt = new ThreadTest();
		tt->SetNum(i);
		tts.push_back(tt);
	}

	{
		pyinit pi;
		for (int i = 0; i < 8; i++)
		{
			for (auto tt : tts)
			{
				tt->Update();
				//std::cout << "my: " << tt->Getmynum() << "\n";
				//std::cout << "va: " << tt->nextnum << "\n";
			}
		}
	}

	for (auto tt : tts)
	{
		delete tt;
	}
}

int main()
{
	threexe();
	//ts();
	return 0;
}
