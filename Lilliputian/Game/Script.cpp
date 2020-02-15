#include "Script.hpp"

Lilliputian::Script::Script(ScriptingAPI* scriptingAPI)
{
	this->scriptingAPI = scriptingAPI;
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
	if (this->startLogic)
		this->startLogic(this->scriptingAPI);
}

void Lilliputian::Script::executeInputLogic()
{
	if (this->inputLogic)
		this->inputLogic(this->scriptingAPI);
}

void Lilliputian::Script::executeFrameLogic()
{
	if (this->frameLogic)
		this->frameLogic(this->scriptingAPI);
}

void Lilliputian::Script::executeComputeLogic()
{
	if (this->computeLogic)
		this->computeLogic(this->scriptingAPI);
}

void Lilliputian::Script::executeLateLogic()
{
	if (this->lateLogic)
		this->lateLogic(this->scriptingAPI);
}

void Lilliputian::Script::executeFinalLogic()
{
	if (this->finalLogic)
		this->finalLogic(this->scriptingAPI);
}