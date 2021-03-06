/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

// Test that editing a mixed-case attribute preserves the case

const TEST_URL = URL_ROOT + "doc_markup_svg_attributes.html";

add_task(function* () {
  let {inspector, testActor} = yield openInspectorForURL(TEST_URL);

  yield inspector.markup.expandAll();
  yield selectNode("svg", inspector);

  yield testWellformedMixedCase(inspector, testActor);
  yield testMalformedMixedCase(inspector, testActor);
});

function* testWellformedMixedCase(inspector, testActor) {
  info("Modifying a mixed-case attribute, " +
    "expecting the attribute's case to be preserved");

  info("Listening to markup mutations");
  let onMutated = inspector.once("markupmutation");

  info("Focusing the viewBox attribute editor");
  let {editor} = yield focusNode("svg", inspector);
  let attr = editor.attrElements.get("viewBox").querySelector(".editable");
  attr.focus();
  EventUtils.sendKey("return", inspector.panelWin);

  info("Editing the attribute value and waiting for the mutation event");
  let input = inplaceEditor(attr).input;
  input.value = "viewBox=\"0 0 1 1\"";
  EventUtils.sendKey("return", inspector.panelWin);
  yield onMutated;

  yield assertAttributes("svg", {
    "viewBox": "0 0 1 1",
    "width": "200",
    "height": "200"
  }, testActor);
}

function* testMalformedMixedCase(inspector, testActor) {
  info("Modifying a malformed, mixed-case attribute, " +
    "expecting the attribute's case to be preserved");

  info("Listening to markup mutations");
  let onMutated = inspector.once("markupmutation");

  info("Focusing the viewBox attribute editor");
  let {editor} = yield focusNode("svg", inspector);
  let attr = editor.attrElements.get("viewBox").querySelector(".editable");
  attr.focus();
  EventUtils.sendKey("return", inspector.panelWin);

  info("Editing the attribute value and waiting for the mutation event");
  let input = inplaceEditor(attr).input;
  input.value = "viewBox=\"<>\"";
  EventUtils.sendKey("return", inspector.panelWin);
  yield onMutated;

  yield assertAttributes("svg", {
    "viewBox": "<>",
    "width": "200",
    "height": "200"
  }, testActor);
}
