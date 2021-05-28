#include <stdexcept>
#include <assert.h>
#include <string>
#include "GameSettings.h"
#include "InputHandler.h"
#include "GameSession.h"
#include "GameMap.h"
#include "AGhost.h"
#include "Player.h"
#include "vec2.h"

GameSession::GameSession(const char* gameMap, Screen* screen, InputHandler* input)
{
	this->screen = screen;
	this->input = input;
	drawOffset = vec2();
	player = new Player(vec2(), this, input);
	ghosts = vector<AGhost*>();
	map = new GameMap(gameMap);
	livesLeft =  DEFAULT_LIVES;
	level = 0;
	SetupLevel();
}

GameSession::~GameSession()
{
	delete map;
	for (int i = 0; i < ghosts.size(); i++)
	{
		delete ghosts[i];
	}
	delete player;
}

void GameSession::CheckCollisions()
{
	if (map->GetCollision(player->GetPosition(), COLLISION_DOT))
	{
		map->Destroy(COLLISION_DOT, player->GetPosition());
		score += DOT_SCORE;
	}

	if (map->GetCollision(player->GetPosition(), COLLISION_ENERGIZER))
	{
		for (auto ghost : ghosts) {
			ghost->Frighten(FRIGHTNED_TIME);
		}

		// We reset this value only if player didn't eat energizer before
		// Thus allowing to take 2 energizers and, well, longer combo
		if (player->GetPowerUpTime() <= 0)
			player->SetEatenCount(0);

		player->SetPowerUpTime(FRIGHTNED_TIME);
		map->Destroy(COLLISION_ENERGIZER, player->GetPosition());
		score += ENERGIZER_SCORE;
	}

	for (int i = 0; i < ghosts.size(); i++)
	{
		auto ghost = ghosts[i];
		int ghostState = ghost->GetState();
		if (player->GetPosition() != ghost->GetPosition())
			continue;

		bool powered = player->GetPowerUpTime() > 0;
		if (powered && ghostState == GhostState::Flee)
		{
			ghost->Eat();
			// Add 1 to current count
			int eatCount = player->GetEatenCount() + 1;
			player->SetEatenCount(eatCount);

			// Creates 200, 400, 800, 1600 line
			score += (1 << eatCount) * 100;
			pauseTime = 1000;
		}
		else if (!powered && ghostState != GhostState::Eaten)
		{
			// HACK: This is a hack, to make pause not just after respawn,
			// like at the start of match, but after we died as well
			wantRespawn = true;
			pauseTime += 1000;
			return;
		}
	}

	if (DotsLeft() == 0)
	{
		wantNewLevel = true;
		pauseTime += 2000;
	}
}

void GameSession::SetupLevel()
{
	for (AGhost* ghost : ghosts)
	{
		delete ghost;
	}
	ghosts.clear();

	vec2 playerSpawnPos;
	// Repopulating map with stuff
	map->Setup(level, this, playerSpawnPos, ghosts);

	// Restoring values
	player->SetSpawnPosition(playerSpawnPos);
	player->Respawn();

	time = 0;
	input->ReleaseInputVec();

	// Making a nice pause with ready sign
	pauseTime += 4000;
	drawReadySign = true;
}

bool GameSession::RespawnAll()
{
	livesLeft--;
	pauseTime = 2000;
	player->Respawn();
	time = 0;
	input->ReleaseInputVec();

	for (auto ghost : ghosts)
	{
		ghost->Respawn();
	}

	drawReadySign = livesLeft > 0;
	wantRespawn = false;
	return !drawReadySign;
}

void GameSession::RestartSession()
{
	livesLeft =  DEFAULT_LIVES;
	score = 0;
	level = 0;
	SetupLevel();
}

// Функція повертає true, якщо гру завершено
bool GameSession::Update(int deltaTime)
{
	if (pauseTime > 0)
	{
		pauseTime -= deltaTime;
		DrawToScreen();
		return false;
	}
	else if (wantRespawn)
	{
		return RespawnAll();
	}
	else if (wantNewLevel)
	{
		level++;
		SetupLevel();
		wantNewLevel = false;
		return false;
	}

	drawReadySign = false;
	time += deltaTime;

	player->Update(time, deltaTime);

	// Виклик оновлення через вказівник на базовий
	// клас
	for (auto ghost : ghosts)
	{
		ghost->Update(time, deltaTime);
	}

	CheckCollisions();
	DrawToScreen();

	return false;
}

// Функція, яка переміщує координату у 
// протилежну частину екрана
void GameSession::WrapPos(vec2& pos)
{
	vec2 mapSize = map->GetMapSize();

	if (pos.x < 0)
		pos.x = mapSize.x - 1;
	else if (pos.x > mapSize.x - 1)
		pos.x = 0;
}

int GameSession::DotsLeft()
{
	return map->GetCount(COLLISION_DOT);
}

int GameSession::GetScore() {
	return score;
}

void GameSession::DrawToScreen()
{
	// Lame
	int mapSizeX, mapSizeY;
	vec2 mapSize = map->GetMapSize();
	mapSizeX = mapSize.x;
	mapSizeY = mapSize.y;

	vec2 drawOffset = this->drawOffset;

	// Printing score
	string text = "Score:";
	vec2 center = vec2{ (mapSizeX / 2) + 1, 0 };
	vec2 scoreOffset = drawOffset + center;
	text += std::to_string(this->score);

	screen->PrintText(scoreOffset, text);

	text = "Lvl ";
	text += std::to_string(level);

	screen->PrintText(drawOffset, text);

	// Printing Lives Icons
	for (int i = 0; i <  DEFAULT_LIVES; i++)
	{
		auto pos = vec2{ -1, mapSizeY - i - 1 } + drawOffset;
		if (i < livesLeft) {
			screen->SetPixel(pos, '@');
			screen->SetColor(pos, COL_YELLOW);
		}
		else
		{
			screen->SetPixel(pos, ' ');
			screen->SetColor(pos, COL_RESET);
		}
	}

	drawOffset.y += 1;

	// Clearing our pixels first
	// Which is lame, should optimize it.
	map->Draw(screen, drawOffset);

	// Drawing player
	screen->SetPixel(player->GetPosition() + drawOffset, PLAYER_CHAR);
	const char* col = COL_YELLOW;
	screen->SetColor(player->GetPosition() + drawOffset, col);

	// Drawing ghosts (They are ontop of the player)

	for (int i = 0; i < ghosts.size(); i++)
	{
		auto ghostDrawPos = ghosts[i]->GetPosition() + drawOffset;
		screen->SetPixel(ghostDrawPos, 'W');

		const char* ghostCol;

		switch (ghosts[i]->GetState())
		{
		default:
			ghostCol = ghosts[i]->GetGhostColor();
			break;
		case GhostState::Flee:
			ghostCol = COL_BRIGHT_BLUE;
			break;
		case GhostState::Eaten:
			ghostCol = COL_WHITE;
			break;
		}

		screen->SetColor(ghostDrawPos, ghostCol);
	}

	// Ready sign
	if (drawReadySign)
	{
		text = "READY";
	}
	else {
		text = "     ";
	}

	vec2 pos = vec2{ mapSizeX, 1 };
	screen->PrintTextVertical(pos + drawOffset, text, COL_BRIGHT_YELLOW);
}

void GameSession::SetDrawOffset(vec2 value) {
	drawOffset = value;
}

vec2 GameSession::GetGhostPosition(int name)
{
	for (size_t i = 0; i < ghosts.size(); i++)
	{
		if (ghosts[i]->GetGhostName() == name) {
			return ghosts[i]->GetPosition();
		}
	}

	throw std::invalid_argument("Name is wrong");
}

GameMap* GameSession::GetMap() {
	return map;
}

int GameSession::GetLevel()
{
	return level;
}

vec2 GameSession::GetPlayerDirection() {
	return player->GetDirection();
}

vec2 GameSession::GetPlayerPosition() {
	return player->GetPosition();
}
