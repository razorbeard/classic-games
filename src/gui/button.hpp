#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "component.hpp"
#include "resources/resource_identifiers.hpp"
#include "state_stack/state.hpp"

#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

namespace GUI
{
	class Button : public Component
	{
	public:
		typedef std::shared_ptr<Button>	Ptr;
		typedef std::function<void()> Callback;

		enum Type
		{
			Normal,
			Selected,
			Pressed,
			TypeCount
		};

	public:
		Button(State::Context context);

		void setCallback(Callback callback);
		void setText(const std::string& text);
		void setText(const std::string& text, unsigned int size);
		void setText(const std::string& text, unsigned int size, float thickness);
		void setToggle(bool flag);

		virtual bool isSelectable() const override;
		virtual void select() override;
		virtual void deselect() override;

		virtual void activate() override;
		virtual void deactivate() override;

		virtual void update(sf::Time dt) override;
		virtual void handleEvent(const sf::Event& event) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void changeTexture(Type buttonType);

	protected:
		SoundPlayer& mSounds;
		Callback mCallback;
		bool mIsToggle;
		sf::Text mText;
		sf::Time mTimer;
		Type mCurrentType;
		bool mShowText;
	};
}

#endif // BUTTON_HPP