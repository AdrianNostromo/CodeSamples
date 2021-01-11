"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Pos3D_1 = require("base/util/Pos3D");
const Quaternion3D_1 = require("base/util/Quaternion3D");
class BasicWorldEntityBase {
    constructor(template, entityId, handler) {
        this.modulesList = [];
        this.modulesList_withGameLoop = [];
        this.modulesMap = new Map();
        this.pos = new Pos3D_1.Pos3D(0, 0, 0);
        this.rot = new Quaternion3D_1.Quaternion3D();
        this.ownerPlayer = null;
        // This is edited only by GWIEntities.
        this.isMarkedForRemoval = false;
        this.template = template;
        this.entityId = entityId;
        this.handler = handler;
    }
    getInstanceId() {
        return this.instanceId;
    }
    setInstanceId(instanceId) {
        this.instanceId = instanceId;
    }
    getPos() {
        return this.pos;
    }
    getX() {
        return this.pos.x;
    }
    getY() {
        return this.pos.y;
    }
    getZ() {
        return this.pos.z;
    }
    setPos(pos) {
        this.setPosXYZ(pos.x, pos.y, pos.z);
    }
    setPosXYZ(x, y, z, skipSnapshotCheck = false) {
        if (this.pos.equalsXYZ(x, y, z)) {
            return;
        }
        this.pos.setXYZ(x, y, z);
        if (!skipSnapshotCheck) {
            this.onPosChanged();
        }
    }
    onPosChanged() {
        //void
    }
    getRot() {
        return this.rot;
    }
    setRot(rot, skipSnapshotCheck = false) {
        if (this.rot.equalsQ(rot)) {
            return;
        }
        this.rot.setQ(rot);
        if (!skipSnapshotCheck) {
            this.onRotDChanged();
        }
    }
    setRotAxisAngle(axis, degrees, skipSnapshotCheck = false) {
        this.rot.setAxisAngle(axis, degrees);
        if (!skipSnapshotCheck) {
            this.onRotDChanged();
        }
    }
    onRotDChanged() {
        //void
    }
    getOwnerPlayer() {
        return this.ownerPlayer;
    }
    setOwnerPlayer(ownerPlayer) {
        if (this.ownerPlayer == ownerPlayer) {
            return;
        }
        this.ownerPlayer = ownerPlayer;
        this.onOwnerPlayerChanged();
    }
    onOwnerPlayerChanged() {
        //void
    }
    onEntityRemove_pre() {
        //void
    }
    // The modules register them selfs in their constructor (Module).
    selfAddModule(mod) {
        this.modulesList.push(mod);
        if (mod.usesGameLoop) {
            this.modulesList_withGameLoop.push(mod);
        }
        this.modulesMap.set(mod.moduleId, mod);
    }
    getModule(moduleId) {
        let mod = this.modulesMap.get(moduleId);
        return mod;
    }
}
exports.BasicWorldEntityBase = BasicWorldEntityBase;
//# sourceMappingURL=BasicWorldEntityBase.js.map