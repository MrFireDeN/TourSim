// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Appearance/AgentAppearanceTrait.h"
#include "MassEntityTemplateRegistry.h"
#include "Gameplay/Agents/Appearance/AppearanceAppliedFragment.h"

void UAgentAppearanceTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FAgentAppearanceFragment>();
	BuildContext.AddFragment<FAppearanceAppliedFragment>();
}
