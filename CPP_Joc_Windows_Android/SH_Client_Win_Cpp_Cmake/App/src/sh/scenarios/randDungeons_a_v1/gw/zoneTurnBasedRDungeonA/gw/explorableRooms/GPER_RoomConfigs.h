#pragma once

#include <base/gh.h>
#include "GPER_Base.h"
#include <memory>
#include <base/list/ArrayList.h>

class IWorldEntity;
class EntityTemplate;
namespace worldGame3D {
    class ContentGroup;
};
namespace graphics {
    class TilingInformation;
};

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class GPER_RoomConfigs : public GPER_Base {priv typedef GPER_Base super;pub dCtor(GPER_RoomConfigs);
    prot class Room {pub dCtor(Room);
        pub class Door {pub dCtor(Door);
            // This class is used to hold visuals.
            pub DungeonContent::LevelZone::Room::Door* config;

            pub Room* otherSideRoom = nullptr;

            pub explicit Door(DungeonContent::LevelZone::Room::Door* config)
                : config(config)
            {}
        };

        pub int roomIndex;
        pub std::shared_ptr<DungeonContent::LevelZone::Room> roomConfig;

        pub ArrayList<Door> doorsList{};

        pub bool isContentCreated = false;
        pub bool isContentEnabled = false;
        pub bool isVisitedByPlayer = false;

        // Use a list because the createEntity param is a list.
        pub ArrayList<std::shared_ptr<worldGame3D::ContentGroup>> contentGroupsList{};

        pub explicit Room(int roomIndex, std::shared_ptr<DungeonContent::LevelZone::Room>& roomConfig)
            : roomIndex(roomIndex), roomConfig(roomConfig)
        {}
    };

    prot std::shared_ptr<DungeonContent::LevelZone> levelZoneConfig;
    // NOTE. This must not be modified after init.
    prot ArrayList<Room> roomsList{};

    // This contains a grid for the entire zone and has each cell set to the roomIndex for it.
    // This is used to detect active room.
    prot std::shared_ptr<Array2D<short>> levelZoneRoomIndexesGrid = nullptr;

    pub explicit GPER_RoomConfigs(base::IGameWorld* gw);

    pub void initLevelZoneRooms(std::shared_ptr<DungeonContent::LevelZone> levelZoneConfig) final;
    prot void createRoomContent(Room& room, bool isContentEnabled);
    priv IWorldEntity* createEnvironmentEntity(
        Vector3& pos,
        float rotZ,
        EntityTemplate* entityTemplate,
        std::shared_ptr<std::string> instanceId,
        base::IGameWorld* gw, Room& room, bool isContentEnabled,
        ArrayList<Visual3DModelMaterial*>* materialsList, std::string extraDataId_materialsList,
        std::shared_ptr<graphics::TilingInformation> tilingInformation);
    priv IWorldEntity* createMobEntity(
        Vector3& pos,
        float rotZ,
        EntityTemplate* entityTemplate,
        ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* impactPayloadsList,
        base::IGameWorld* gw, Room& room, bool isContentEnabled);


    pub ~GPER_RoomConfigs() override;
};
};
};
