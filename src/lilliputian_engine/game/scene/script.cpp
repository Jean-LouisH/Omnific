#include "script.hpp"

Lilliputian::Script::Script(ScriptingAPIs* scriptingAPIs, const char* scriptName)
{
	this->scriptingAPIs = scriptingAPIs;
	this->name = scriptName;

	this->startLogic = nullptr;
	this->inputLogic = nullptr;
	this->frameLogic = nullptr;
	this->computeLogic = nullptr;
	this->lateLogic = nullptr;
	this->finalLogic = nullptr;
}

void Lilliputian::Script::addStartLogic(ScriptLogicCallback startLogic)
{
	this->startLogic = startLogic;
}

void Lilliputian::Script::addInputLogic(ScriptLogicCallback inputLogic)
{
	this->inputLogic = inputLogic;
}

void Lilliputian::Script::addFrameLogic(ScriptLogicCallback frameLogic)
{
	this->frameLogic = frameLogic;
}

void Lilliputian::Script::addComputeLogic(ScriptLogicCallback computeLogic)
{
	this->computeLogic = computeLogic;
}

void Lilliputian::Script::addLateLogic(ScriptLogicCallback lateLogic)
{
	this->lateLogic = lateLogic;
}
void Lilliputian::Script::addFinalLogic(ScriptLogicCallback finalLogic)
{
	this->finalLogic = finalLogic;
}

void Lilliputian::Script::executeStartLogic()
{
	if (this->startLogic != nullptr)
		this->startLogic(*this->scriptingAPIs);
}

void Lilliputian::Script::executeInputLogic()
{
	if (this->inputLogic != nullptr)
		this->inputLogic(*this->scriptingAPIs);
}

void Lilliputian::Script::executeFrameLogic()
{
	if (this->frameLogic != nullptr)
		this->frameLogic(*this->scriptingAPIs);
}

void Lilliputian::Script::executeComputeLogic()
{
	if (this->computeLogic != nullptr)
		this->computeLogic(*this->scriptingAPIs);
}

void Lilliputian::Script::executeLateLogic()
{
	if (this->lateLogic != nullptr)
		this->lateLogic(*this->scriptingAPIs);
}

void Lilliputian::Script::executeFinalLogic()
{
	if (this->finalLogic != nullptr)
		this->finalLogic(*this->scriptingAPIs);
}