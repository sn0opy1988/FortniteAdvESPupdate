#ifndef __ESP_H
#define __ESP_H

#include <string>
#include <sstream>

#include "config.h"
#include "global.h"
#include "util.h"
#include "renderer.h"

class ESP
{

public:
	void Draw(Config& cfg, Renderer& renderer)
	{
		if (!cfg.m_EnableESP)
		{
			return;
		}

		if (Global::m_PersistentLevel == nullptr || Global::m_LocalPlayer == nullptr)
		{
			return;
		}

		if (Global::m_LocalPlayer == nullptr || Global::m_LocalPlayer->PlayerController == nullptr)
		{
			return;
		}

		auto playerController = Global::m_LocalPlayer->PlayerController;
		if (playerController->AcknowledgedPawn == nullptr)
		{
			return;
		}

		auto localPos = playerController->AcknowledgedPawn->RootComponent->Location;

		auto actors = Global::m_PersistentLevel->AActors;
		for (int i = 0; i < actors.Num(); i++)
		{
			if (!actors.IsValidIndex(i))
			{
				continue;
			}

			auto actor = Global::m_PersistentLevel->AActors[i];
			if (actor == nullptr || actor->RootComponent == nullptr)
			{
				continue;
			}

			if (actor->IsA(SDK::AFortPawn::StaticClass()))
			{
				auto pawn = static_cast<SDK::AFortPawn*>(actor);
				if (pawn->GetName().find("PlayerPawn_Athena_C") == std::string::npos)
				{
					continue;
				}

				if (Util::IsTeammate(actor) || Util::IsLocalPlayer(actor))
				{
					continue;
				}

				if (pawn->PlayerState == nullptr || !pawn->PlayerState->IsA(SDK::AFortPlayerStateAthena::StaticClass()))
				{
					continue;
				}

				auto state = static_cast<SDK::AFortPlayerStateAthena*>(pawn->PlayerState);
				if (!state->PlayerName.IsValid() || pawn->RootComponent == nullptr)
				{
					continue;
				}

				SDK::FVector playerLoc;
				Util::Engine::GetBoneLocation(pawn->Mesh, &playerLoc, 66);

				SDK::FVector2D screenPos;
				if (!Util::Engine::WorldToScreen(Global::m_LocalPlayer->PlayerController, playerLoc, &screenPos))
				{
					continue;
				}

				auto name = state->PlayerName.c_str();

				auto distance = Util::GetDistance(localPos, pawn->RootComponent->Location);
				std::wstringstream ss;
				ss << name << L" [" << Util::DistanceToString(distance) << L"]";

				// Extended ESP stuff
				if (cfg.m_EnableExtendedESP)
				{
					auto playerHealth = state->CurrentHealth;
					float playerShield = state->CurrentShield;
					std::wstring playerWeapon = L"No Weapon";
					int ammoCount = 0;
					int weaponTier = 0;

					auto curWeapon = pawn->CurrentWeapon;
					if (curWeapon != nullptr && curWeapon->WeaponData != nullptr)
					{
						auto itemDef = static_cast<SDK::UFortWorldItemDefinition*>(curWeapon->WeaponData);

						ammoCount = curWeapon->AmmoCount;
						if (itemDef->ItemType == SDK::EFortItemType::WeaponRanged
							|| itemDef->ItemType == SDK::EFortItemType::WeaponMelee
							|| itemDef->ItemType == SDK::EFortItemType::WeaponHarvest)
						{
							playerWeapon = itemDef->DisplayName.Get();
							weaponTier = static_cast<int>(itemDef->Tier.GetValue());
						}
					}

					std::wstringstream extendedString;
					extendedString << L"\nWP:" << playerWeapon << L" (T" << weaponTier << L")";
					extendedString << L"\nHP:" << playerHealth << L" SH:" << playerShield;
					extendedString << L"\nAM:" << ammoCount;
					ss << extendedString.str();
				}

				auto size = renderer.getTextExtent(ss.str(), 12.0f, DEFAULT_FONT);
				renderer.drawText(Vec2(screenPos.X - size.x * 0.5f, screenPos.Y - size.y - 16.0f), ss.str(), ENEMY_TEXT_COLOR, 0, 12.0f, DEFAULT_FONT);
			}
			else if (actor->IsA(SDK::AB_Pickups_C::StaticClass()))
			{
				SDK::FVector2D screenPos;
				if (!Util::Engine::WorldToScreen(Global::m_LocalPlayer->PlayerController, actor->RootComponent->Location, &screenPos))
				{
					continue;
				}

				auto pickup = static_cast<SDK::AB_Pickups_C*>(actor);

				if (!pickup->ItemDefinition)
				{
					continue;
				}

				auto itemDef = pickup->ItemDefinition;
				if (itemDef->ItemType == SDK::EFortItemType::Ammo)
				{
					continue;
				}

				Color color{ 0.8f, 0.8f, 0.8f, 0.95f };

				switch (itemDef->Tier.GetValue())
				{
				case SDK::EFortItemTier::I: // white
					break;
				case SDK::EFortItemTier::II: // green
					color = Color{ 0.0f, 0.95f, 0.0f, 0.95f };
					break;
				case SDK::EFortItemTier::III: // blue
					color = Color{ 0.4f, 0.65f, 1.0f, 0.95f };
					break;
				case SDK::EFortItemTier::IV: // purple
					color = Color{ 0.7f, 0.25f, 0.85f, 0.95f };
					break;
				case SDK::EFortItemTier::V: // orange
					color = Color{ 0.85f, 0.65f, 0.0f, 0.95f };
					break;
				case SDK::EFortItemTier::VI: // gold
				case SDK::EFortItemTier::VII:
					color = Color{ 0.95f, 0.85f, 0.45f, 0.95f };
					break;
				case SDK::EFortItemTier::VIII:
				case SDK::EFortItemTier::IX:
				case SDK::EFortItemTier::X:
					color = Color{ 1.0f, 0.0f, 1.0f, 0.95f };
					break;
				}

				if (!itemDef->DisplayName.Get())
				{
					continue;
				}

				std::wstring name = itemDef->DisplayName.Get();
				if (name == L"Bandage" || name == L"Medkit")
				{
					color = Color{ 0.9f, 0.55f, 0.55f, 0.95f };
				}
				else if (name == L"Shield Potion")
				{
					color = Color{ 0.35f, 0.55f, 0.85f, 0.95f };
				}

				if (pickup->RootComponent == nullptr)
				{
					continue;
				}

				auto distance = Util::GetDistance(localPos, pickup->RootComponent->Location);
				std::wstringstream ss;

				if (cfg.m_EnableExtendedESP)
				{
					ss << L"(T" << static_cast<int>(itemDef->Tier.GetValue()) << L") ";
				}

				ss << name << L" [" << Util::DistanceToString(distance) << L"]";

				auto size = renderer.getTextExtent(ss.str(), 11.0f, DEFAULT_FONT);
				renderer.drawText(Vec2(screenPos.X - size.x, screenPos.Y - size.y), ss.str(), color, 0, 11.0f, DEFAULT_FONT);
			}
			else if (actor->GetName().find("AthenaSupplyDrop_02_C") != std::string::npos)
			{
				SDK::FVector2D screenPos;
				if (!Util::Engine::WorldToScreen(Global::m_LocalPlayer->PlayerController, actor->RootComponent->Location, &screenPos))
				{
					continue;
				}

				auto size = renderer.getTextExtent(L"Supply Drop", 12.0f, DEFAULT_FONT);
				renderer.drawText(Vec2(screenPos.X - size.x, screenPos.Y - size.y), L"Supply Drop", Color{ 0.4f, 0.9f, 0.4f, 0.4f }, 0, 12.0f, DEFAULT_FONT);
			}
		}
	}

private:

};

#endif
