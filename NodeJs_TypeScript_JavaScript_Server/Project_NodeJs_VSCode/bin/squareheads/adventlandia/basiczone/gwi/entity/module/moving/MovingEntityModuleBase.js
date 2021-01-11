"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ModuleLocal_1 = require("../ModuleLocal");
const MathUtil_1 = require("base/util/MathUtil");
const Vector3_1 = require("base/util/Vector3");
const Quaternion3D_1 = require("base/util/Quaternion3D");
class MovingEntityModuleBase extends ModuleLocal_1.ModuleLocal {
    constructor(entity, template) {
        super(template.moduleId, entity, true);
        this.isMoveInProgress = false;
        this.isTurnInProgress = false;
        this.isMovementStopInProgress = false;
        this.template = template;
    }
    getIsMoveInProgress() {
        return this.isMoveInProgress;
    }
    getDestMoveX() {
        return this.destMoveX;
    }
    getDestMoveY() {
        return this.destMoveY;
    }
    getIsTurnInProgress() {
        return this.isTurnInProgress;
    }
    getDdestTurnDZ() {
        return this.destTurnDZ;
    }
    getIsMovementStopInProgress() {
        return this.isMovementStopInProgress;
    }
    move(destMoveX, destMoveY) {
        if (this.isMoveInProgress && this.destMoveX == destMoveX && this.destMoveY == destMoveY) {
            // Ignore duplicate.
            return;
        }
        this.destMoveX = destMoveX;
        this.destMoveY = destMoveY;
        this.isMoveInProgress = true;
    }
    stopMovement() {
        if (this.isMovementStopInProgress) {
            // Ignore duplicate.
            return;
        }
        // Stop the actual action.
        this.isMovementStopInProgress = true;
    }
    turn(destTurnDZ) {
        if (this.isTurnInProgress && this.destTurnDZ == destTurnDZ) {
            // Ignore duplicate.
            return;
        }
        this.destTurnDZ = destTurnDZ;
        this.isTurnInProgress = true;
    }
    gameLoop(deltaS, deltaMS) {
        super.gameLoop(deltaS, deltaMS);
        if (this.isMovementStopInProgress) {
            this.isMovementStopInProgress = false;
            if (this.isMoveInProgress) {
                this.destMoveX = this.getEntity().getX();
                this.destMoveY = this.getEntity().getY();
                this.isMoveInProgress = false;
            }
        }
        if (this.isMoveInProgress) {
            let currentX = this.getEntity().getX();
            let currentY = this.getEntity().getY();
            let tickMoveAmount = this.template.moveSpeedS * deltaS;
            let delta = MathUtil_1.MathUtil.dist(currentX, currentY, this.destMoveX, this.destMoveY);
            let newX;
            let newY;
            let angRZ;
            if (currentX == this.destMoveX && currentY == this.destMoveY) {
                angRZ = MathUtil_1.MathUtil.toRadians(this.getEntity().getRot().getAngleAroundAxis(Vector3_1.Vector3.Z));
            }
            else {
                angRZ = Math.atan2(this.destMoveY - currentY, this.destMoveX - currentX);
            }
            if (tickMoveAmount >= delta) {
                newX = this.destMoveX;
                newY = this.destMoveY;
                this.isMoveInProgress = false;
            }
            else {
                newX = currentX + Math.cos(angRZ) * tickMoveAmount;
                newY = currentY + Math.sin(angRZ) * tickMoveAmount;
            }
            // This will not cause the snapshot flag to be set for movement 
            // because the movement flag update handles that.
            this.getEntity().setPosXYZ(newX, newY, this.getEntity().getZ(), true);
            if (!this.isTurnInProgress) {
                // This will not cause the snapshot flag to be set for movement 
                // because the movement flag update handles that.
                this.getEntity().setRot(Quaternion3D_1.Quaternion3D.fromAxisAngle(Vector3_1.Vector3.Z, MathUtil_1.MathUtil.toDegrees(angRZ)), true);
            }
        }
        if (this.isTurnInProgress) {
            this.isTurnInProgress = false;
            this.getEntity().setRotAxisAngle(Vector3_1.Vector3.Z, this.destTurnDZ, true);
        }
    }
}
exports.MovingEntityModuleBase = MovingEntityModuleBase;
//# sourceMappingURL=MovingEntityModuleBase.js.map