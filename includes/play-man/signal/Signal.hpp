// ****************************************************************************** //
//   _______   __                              __       __                        //
//  /       \ /  |                            /  \     /  |                       //
//  $$$$$$$  |$$ |  ______   __    __         $$  \   /$$ |  ______   _______     //
//  $$ |__$$ |$$ | /      \ /  |  /  | ______ $$$  \ /$$$ | /      \ /       \    //
//  $$    $$/ $$ | $$$$$$  |$$ |  $$ |/      |$$$$  /$$$$ | $$$$$$  |$$$$$$$  |   //
//  $$$$$$$/  $$ | /    $$ |$$ |  $$ |$$$$$$/ $$ $$ $$/$$ | /    $$ |$$ |  $$ |   //
//  $$ |      $$ |/$$$$$$$ |$$ \__$$ |        $$ |$$$/ $$ |/$$$$$$$ |$$ |  $$ |   //
//  $$ |      $$ |$$    $$ |$$    $$ |        $$ | $/  $$ |$$    $$ |$$ |  $$ |   //
//  $$/       $$/  $$$$$$$/  $$$$$$$ |        $$/      $$/  $$$$$$$/ $$/   $$/    //
//                          /  \__$$ |                                            //
//                          $$    $$/                                             //
//                           $$$$$$/                                              //
//                                                                                //
//                            By: K1ngmar and rvan-mee                            //
// ****************************************************************************** //

#pragma once

#include <vector>
#include <functional>

template <class>
class Signal;

/**
 * @brief Signal slot implementation, callback aka targets can be added by calling Connect.
 * 		  Targets can be removed by callin Disconnect.
 * 
 * @note Callback are executed in the order they were added.
 */
template <class R, class ...Args>
class Signal<R(Args...)>
{
	using CallbackType = std::function<R(Args...)>;

	std::vector<CallbackType> callbacks;

public:

	/**
	 * @brief Registers a callback for the signal.
	 * 
	 * @param callBackToAdd 
	 */
	void Connect(const CallbackType& callBackToAdd)
	{
		callbacks.emplace_back(callBackToAdd);
	}

	/**
	 * @brief 
	 * 
	 * @param callBackToRemove 
	 */
	void Disconnect(const CallbackType& callBackToRemove)
	{
		std::erase_if(callbacks, [&](const CallbackType& element)
		{
			return element.target_type() == callBackToRemove.target_type();
		});
	}

	/**
	 * @brief
	 * 
	 * @param args 
	 */
	void operator () (Args... args)
	{
		for (auto& callback : callbacks)
		{
			callback(args...);
		}
	}

};
