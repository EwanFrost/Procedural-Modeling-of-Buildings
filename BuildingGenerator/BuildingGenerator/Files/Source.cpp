#include "Parser\Parser.h"
#include "Modeler\Modeler.h"
#include "Renderer\Renderer.h"

int main(int argc, char** argv)
{
	pars::Parser parser;
	modl::Modeler modeler;
	rend::Renderer renderer;

	parser.Parse();
	modeler.Model(parser.GetRuleSet());
	renderer.Render(modeler.GetDeviationTree());
}