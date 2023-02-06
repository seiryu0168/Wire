#pragma once

template<class C>
class PlayerState
{
	virtual ~PlayerState() {}
	
	//‰Šú‰»(ó‘Ô‚ª•Ï‚í‚é‚²‚Æ‚ÉŒÄ‚Î‚ê‚é)
	virtual void Init(C& player) {}

	virtual void Update(C& player) {}
};