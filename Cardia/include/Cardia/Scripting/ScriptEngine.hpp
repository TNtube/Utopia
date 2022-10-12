﻿#pragma once

#include <Cardia/ECS/Scene.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace Cardia
{
	struct ScriptData
	{
		py::object self;
		std::vector<std::string> onUpdateCallbacks;

		explicit ScriptData(py::object& instance) {
			self = instance;
			onUpdateCallbacks = {};
		}
	};

	struct ScriptEngineData
	{
	public:
		Scene* currentContext;
		std::unordered_map<std::string, ScriptData> classInstances;
		std::vector<std::pair<py::object, std::string>> unRegisteredCallbacks;
		std::vector<py::object> onUpdateFunctions;
		py::object pythonBuiltins;
		py::object cardiaPy;
	};


		class ScriptEngine
		{
		public:
			static void init();
			static void shutdown();
			static void onRuntimeStart(Scene* context);
			static void onRuntimeUpdate();
			static void onRuntimeStop();
			static Scene* getSceneContext();
			static void registerUpdateMethod(py::object& cls, std::string& name);
			static void registerUpdateFunction(py::object& obj);
			static py::object& getPythonBuiltins() { return s_Data->pythonBuiltins; }
		private:
			static std::unique_ptr<ScriptEngineData> s_Data;
			static ScriptData instantiatePythonClass(py::object& classObj, const std::string& id);
		};
}
