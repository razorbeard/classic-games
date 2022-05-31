#include "application.hpp"
#include "state_stack/title_state.hpp"
#include "state_stack/menu_state.hpp"
#include "state_stack/pause_state.hpp"
#include "state_stack/game_over_state.hpp"
#include "state_stack/arkanoid_state.hpp"
#include "state_stack/tetris_state.hpp"

// To avoid strong variations of delta time, and thereby game logic
// problems (collisions for example), we fix the frame rate to 60 FPS
const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 60.0f);

Application::Application()
	: mWindow({ 1024, 768 }, "Classic Games", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayer()
	, mMusic()
	, mSounds()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer, mMusic, mSounds))
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::EightBitArcade, "resources/fonts/8_bit_arcade_in.ttf");
	mFonts.load(Fonts::Sansation, "resources/fonts/sansation.ttf");
	mFonts.load(Fonts::Upheaval, "resources/fonts/upheavtt.ttf");

	mTextures.load(Textures::Buttons, "resources/textures/buttons.png");
	mTextures.load(Textures::Particle, "resources/textures/particles.png");
	mTextures.load(Textures::RingBackground, "resources/textures/ring_background.png");
	mTextures.load(Textures::SelectText, "resources/textures/select_game_text.png");
	mTextures.load(Textures::TitleText, "resources/textures/title_text.png");
	mTextures.load(Textures::WindowIcon, "resources/textures/window_icon.png");

	sf::Image const image{ mTextures.get(Textures::WindowIcon).copyToImage() };
	mWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	mStatisticsText.setFont(mFonts.get(Fonts::Sansation));
	mStatisticsText.setPosition(5.0f, 5.0f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);
}

void Application::run()
{
	sf::Clock clock{};
	sf::Time timeSinceLastUpdate{ sf::Time::Zero };

	// An iteration of this loop is called a frame
	while (mWindow.isOpen())
	{
		// Measure the time each frame take
		sf::Time dt{ clock.restart() };
		timeSinceLastUpdate += dt;

		// Before rendering, we need to verify if the main loop
		// is not going too fast, so we can update game logic
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event{};
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;

	// Update the frame rate
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<GameOverState>(States::GameOver);
	mStateStack.registerState<ArkanoidState>(States::Arkanoid);
	mStateStack.registerState<TetrisState>(States::Tetris);
}
