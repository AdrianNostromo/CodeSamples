#include "DeV_PageGMan.h"
#include "entryUser/IDebugEntryUser.h"
#include <graphics/renderer/geometryManager/combinedRenderable/CombinedRenderable.h>
#include <sstream>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>
#include <graphics/geometry/data/BufferObject.h>
#include <base/list/CompositesBuffer.h>
#include <base/math/Math.h>

using namespace base;

DeV_PageGMan::DeV_PageGMan(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
	: super(app, viewItemConfigs, viewEaseDurationsSList)
{
	//void
}

void DeV_PageGMan::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	cPageIndex = registerNewPage(false/*isDefaultPage*/, true/*usesDynamicEntries*/);

	{
		dtCRends.dde_entriesCount = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dtCRends.dde_entriesCount->setPlaceholderedText(">> Entries count (v i mT rMT):");

		dtCRends.dde_fill = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dtCRends.dde_fill ->setPlaceholderedText(">> Fill% (v i mT rMT):");

		dtCRends.dde_fragmentation = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dtCRends.dde_fragmentation->setPlaceholderedText(">> Fragmentation% (v mT):");

		dtCRends.dde_bufferSubDataCalls = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dtCRends.dde_bufferSubDataCalls->setPlaceholderedText(">> GL Buffer subdata calls (v i mT mNT rMT):");
		
		dtCRends.dde_uploadEntries = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dtCRends.dde_uploadEntries->setPlaceholderedText(">> GL Uplaod entries (v i mT mNT rMT):");
		
		dtCRends.dde_uploadBytes = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dtCRends.dde_uploadBytes->setPlaceholderedText(">> GL Upload bytes (v i mT mNT rMT):");

		dtCRends.dde_defragmentationEntriesChanged = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dtCRends.dde_defragmentationEntriesChanged->setPlaceholderedText(">> Defrag entries changed (v mT):");
	}
}

void DeV_PageGMan::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (isActive() && getActivePageIndex() == cPageIndex) {
		// Enum the existing cRend entries and handle upload.
		int cRendsCount = graphics::CombinedRenderable::globalInstancesList.count();
		if (cRendsCount != oldCRendsCount) {
			oldCRendsCount = cRendsCount;

			dtCRends.dde_entriesCount->setLinesCount(cRendsCount + 2);

			dtCRends.dde_fill->setLinesCount(cRendsCount + 2);
			dtCRends.dde_fragmentation->setLinesCount(cRendsCount + 2);
			
			dtCRends.dde_bufferSubDataCalls->setLinesCount(cRendsCount + 2);
			dtCRends.dde_uploadEntries->setLinesCount(cRendsCount + 2);
			dtCRends.dde_uploadBytes->setLinesCount(cRendsCount + 2);
			
			dtCRends.dde_defragmentationEntriesChanged->setLinesCount(cRendsCount + 2);

			while (dtCRends.datasList.count() < cRendsCount) {
				dtCRends.datasList.append_emplace();
			}

			// Clear all entries because it is no longer valid.
			for (int i = 0; i < cRendsCount; i++) {
				memset(dtCRends.datasList.getPointer(i), 0, sizeof(SingleCRendData));
			}
		}

		{
			// Update datas.
			int i = -1;
			for (ListDL<graphics::CombinedRenderable*>::Entry* entry = graphics::CombinedRenderable::globalInstancesList.getFirst(); entry != nullptr; entry = entry->next) {
				graphics::CombinedRenderable* cRend = entry->data;
				i++;

				SingleCRendData& cRendData = dtCRends.datasList.getReference(i);
				
				{
					cRendData.entriesCount.set_vi(cRend->mm_vertices->bufferObject->getBuffer().capacity() * cRend->mm_vertices->entriesPerGLEntry/*vertices*/, cRend->mm_indices->bufferObject->getBuffer().capacity() * cRend->mm_indices->entriesPerGLEntry/*indices*/);

					cRendData.fillCount.set_vi(cRend->mm_vertices->fillRateCount/*vertices*/, cRend->mm_indices->fillRateCount/*indices*/);
					cRendData.fillPercent.set_vi(static_cast<int>(cRend->mm_vertices->fillRatePercent * 100.0f)/*vertices*/, static_cast<int>(cRend->mm_indices->fillRatePercent * 100.0f)/*indices*/);

					cRendData.fragmentationCount.set_v(cRend->mm_vertices->getTotalFragmentedEntriesCount()/*vertices*/);
					cRendData.fragmentationPercent.set_v(static_cast<int>((((float)cRend->mm_vertices->getTotalFragmentedEntriesCount()) / cRend->mm_vertices->getTotalEntriesCapacity()) * 100.0f)/*vertices*/);

					cRendData.bufferSubDataCalls.set_vi(-1/*vertices*/, -1/*indices*/);
					cRendData.uploadEntries.set_vi(-1/*vertices*/, -1/*indices*/);
					cRendData.uploadBytes.set_vi(-1/*vertices*/, -1/*indices*/);

					cRendData.defragmentationEntriesChanged.set_v(-1/*vertices*/);
				}

				if (cRend->mm_modelTransforms != nullptr) {
					cRendData.entriesCount.set_mTrMT(cRend->mm_modelTransforms->bufferObject->getBuffer().capacity() * cRend->mm_vertices->entriesPerGLEntry/*modelTransforms*/, cRend->mm_remapingModelTransformsIndices->bufferObject->getBuffer().capacity() * cRend->mm_vertices->entriesPerGLEntry/*remapingModelTransforms*/);

					cRendData.fillCount.set_mTrMT(cRend->mm_modelTransforms->fillRateCount/*modelTransforms*/, cRend->mm_remapingModelTransformsIndices->fillRateCount/*remapingModelTransforms*/);
					cRendData.fillPercent.set_mTrMT(static_cast<int>(cRend->mm_modelTransforms->fillRatePercent * 100.0f)/*modelTransforms*/, static_cast<int>(cRend->mm_remapingModelTransformsIndices->fillRatePercent * 100.0f)/*remapingModelTransforms*/);

					cRendData.fragmentationCount.set_mT(cRend->mm_modelTransforms->getTotalFragmentedEntriesCount()/*modelTransforms*/);
					cRendData.fragmentationPercent.set_mT(static_cast<int>((((float)cRend->mm_modelTransforms->getTotalFragmentedEntriesCount()) / cRend->mm_modelTransforms->getTotalEntriesCapacity()) * 100.0f)/*modelTransforms*/);

					cRendData.bufferSubDataCalls.set_mTmNTrMT(-1/*modelTransforms*/, -1/*modelNormalTransforms*/, -1/*remapingModelTransforms*/);
					cRendData.uploadEntries.set_mTmNTrMT(-1/*modelTransforms*/, -1/*modelNormalTransforms*/, -1/*remapingModelTransforms*/);
					cRendData.uploadBytes.set_mTmNTrMT(-1/*modelTransforms*/, -1/*modelNormalTransforms*/, -1/*remapingModelTransforms*/);

					cRendData.defragmentationEntriesChanged.set_mT(-1/*modelTransforms*/);
				}
			}
		}

		std::stringstream ss_entriesCount{};
		
		std::stringstream ss_fill{};
		std::stringstream ss_fragmentation{};
		
		std::stringstream ss_bufferSubDataCalls{};
		std::stringstream ss_uploadEntries{};
		std::stringstream ss_uploadBytes{};
		
		std::stringstream ss_defragmentationEntriesChanged{};

		int i = -1;
		for (ListDL<graphics::CombinedRenderable*>::Entry* entry = graphics::CombinedRenderable::globalInstancesList.getFirst(); entry != nullptr; entry = entry->next) {
			graphics::CombinedRenderable* cRend = entry->data;
			i++;

			SingleCRendData& cRendData = dtCRends.datasList.getReference(i);

			if (i == 0) {
				ss_entriesCount << ">> Entries count:\n|      v|      i|     mT|    rMT|";
				
				ss_fill << ">> Fill:          count          |      percent       |\n|      v|      i|     mT|    rMT|_|   v|   i|  mT| rMT|";
				ss_fragmentation << ">> Fragmentation: count | percent\n|   v|  mT|_|   v|  mT|";
				
				ss_bufferSubDataCalls << ">> GL Buffer subdata calls:\n|   v|   i|  mT| mNT| rMT|";
				ss_uploadEntries << ">> GL Uplaod entries\n|   v|   i|  mT| mNT| rMT|";
				ss_uploadBytes << ">> GL Upload bytes\n|   v|   i|  mT| mNT| rMT|";
				
				ss_defragmentationEntriesChanged << ">> Defrag entries changed:\n|   v|  mT|";
			}

			std::string partsDelimitator = "| ";

			ss_entriesCount << "\n| " << Math::intToPrefixSpacePaddedString(cRendData.entriesCount.vertices, 6) << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.entriesCount.indices, 6);
			
			ss_fill << "\n| " << Math::intToPrefixSpacePaddedString(cRendData.fillCount.vertices, 6) << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fillCount.indices, 6);
			ss_fragmentation << "\n| " << Math::intToPrefixSpacePaddedString(cRendData.fragmentationCount.vertices, 6);
			
			ss_bufferSubDataCalls << "\n| " << "asd" << partsDelimitator << "asd";
			ss_uploadEntries << "\n| " << "asd" << partsDelimitator << "asd";
			ss_uploadBytes << "\n| " << "asd" << partsDelimitator << "asd";
			
			ss_defragmentationEntriesChanged << "\n| " << "asd";

			if (cRend->mm_modelTransforms != nullptr) {
				ss_entriesCount << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.entriesCount.modelTransforms, 6) << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.entriesCount.remapingModelTransforms, 6) << "|";
				
				ss_fill << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fillCount.modelTransforms, 6) << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fillCount.remapingModelTransforms, 6) << "|";
				ss_fragmentation << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fragmentationCount.modelTransforms, 6) << "|";

				ss_bufferSubDataCalls << partsDelimitator << "asd" << partsDelimitator << "asd" << "|";
				ss_uploadEntries << partsDelimitator << "asd" << partsDelimitator << "asd" << "|";
				ss_uploadBytes << partsDelimitator << "asd" << partsDelimitator << "asd" << "|";

				ss_defragmentationEntriesChanged << partsDelimitator << "asd" << "|";
			} else {
				ss_entriesCount << partsDelimitator << "      " << partsDelimitator << "      " << "|";

				ss_fill << partsDelimitator << "      " << partsDelimitator << "      " << "|";
				ss_fragmentation << partsDelimitator << "      " << "|";

				ss_bufferSubDataCalls << partsDelimitator << "asd" << partsDelimitator << "asd" << "|";
				ss_uploadEntries << partsDelimitator << "asd" << partsDelimitator << "asd" << "|";
				ss_uploadBytes << partsDelimitator << "asd" << partsDelimitator << "asd" << "|";

				ss_defragmentationEntriesChanged << partsDelimitator << "asd" << "|";
			}

			// Second group column;
			ss_fill << "_| " << Math::intToPrefixSpacePaddedString(cRendData.fillPercent.vertices, 3) << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fillPercent.indices, 3);
			ss_fragmentation << "_| " << Math::intToPrefixSpacePaddedString(cRendData.fragmentationPercent.vertices, 3);

			if (cRend->mm_modelTransforms != nullptr) {
				ss_fill << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fillPercent.modelTransforms, 3) << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fillPercent.remapingModelTransforms, 3) << "|";
				ss_fragmentation << partsDelimitator << Math::intToPrefixSpacePaddedString(cRendData.fragmentationPercent.modelTransforms, 3) << "|";
			} else {
				ss_fill << partsDelimitator << "   " << partsDelimitator << "   " << "|";
				ss_fragmentation << partsDelimitator << "      " << "|";
			}

		}

		dtCRends.dde_entriesCount->setPlaceholderedText(ss_entriesCount.str());

		dtCRends.dde_fill->setPlaceholderedText(ss_fill.str());
		dtCRends.dde_fragmentation->setPlaceholderedText(ss_fragmentation.str());

		dtCRends.dde_bufferSubDataCalls->setPlaceholderedText(ss_bufferSubDataCalls.str());
		dtCRends.dde_uploadEntries->setPlaceholderedText(ss_uploadEntries.str());
		dtCRends.dde_uploadBytes->setPlaceholderedText(ss_uploadBytes.str());

		dtCRends.dde_defragmentationEntriesChanged->setPlaceholderedText(ss_defragmentationEntriesChanged.str());

		if (isMinMaxCumulativeClearRequested) {
			isMinMaxCumulativeClearRequested = false;
		}
	}
}

void DeV_PageGMan::clearMinMaxCumulative() {
	super::clearMinMaxCumulative();

	isMinMaxCumulativeClearRequested = true;
}

void DeV_PageGMan::disposeMainPre() {
	if (dtCRends.dde_entriesCount != nullptr) {
		dtCRends.dde_entriesCount->remove();
		dtCRends.dde_entriesCount = nullptr;
	}
	if (dtCRends.dde_fill != nullptr) {
		dtCRends.dde_fill->remove();
		dtCRends.dde_fill = nullptr;
	}
	if (dtCRends.dde_fragmentation != nullptr) {
		dtCRends.dde_fragmentation->remove();
		dtCRends.dde_fragmentation = nullptr;
	}
	if (dtCRends.dde_bufferSubDataCalls != nullptr) {
		dtCRends.dde_bufferSubDataCalls->remove();
		dtCRends.dde_bufferSubDataCalls = nullptr;
	}
	if (dtCRends.dde_uploadEntries != nullptr) {
		dtCRends.dde_uploadEntries->remove();
		dtCRends.dde_uploadEntries = nullptr;
	}
	if (dtCRends.dde_uploadBytes != nullptr) {
		dtCRends.dde_uploadBytes->remove();
		dtCRends.dde_uploadBytes = nullptr;
	}
	if (dtCRends.dde_defragmentationEntriesChanged != nullptr) {
		dtCRends.dde_defragmentationEntriesChanged->remove();
		dtCRends.dde_defragmentationEntriesChanged = nullptr;
	}

	super::disposeMainPre();
}

DeV_PageGMan::~DeV_PageGMan() {
	//void
}
