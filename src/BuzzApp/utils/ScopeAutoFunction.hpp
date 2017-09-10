#pragma once

template<class T>
class ScopeAutoFunction {
	T _fx;
public:
	
	ScopeAutoFunction(const T& fx) {
		_fx = fx;
	}
	
	ScopeAutoFunction(T&& fx) {
		_fx = fx;
	}

	~ScopeAutoFunction() {
		_fx();
	}
};