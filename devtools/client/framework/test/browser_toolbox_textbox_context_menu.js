/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

const URL = "data:text/html;charset=utf8,test for textbox context menu";

add_task(function* () {
  let toolbox = yield openNewTabAndToolbox(URL, "inspector");
  let textboxContextMenu = toolbox.textBoxContextMenuPopup;

  emptyClipboard();

  // Make sure the focus is predictable.
  let inspector = toolbox.getPanel("inspector");
  let onFocus = once(inspector.searchBox, "focus");
  inspector.searchBox.focus();
  yield onFocus;

  ok(textboxContextMenu, "The textbox context menu is loaded in the toolbox");

  let cmdUndo = textboxContextMenu.querySelector("[command=cmd_undo]");
  let cmdDelete = textboxContextMenu.querySelector("[command=cmd_delete]");
  let cmdSelectAll = textboxContextMenu.querySelector("[command=cmd_selectAll]");
  let cmdCut = textboxContextMenu.querySelector("[command=cmd_cut]");
  let cmdCopy = textboxContextMenu.querySelector("[command=cmd_copy]");
  let cmdPaste = textboxContextMenu.querySelector("[command=cmd_paste]");

  info("Opening context menu");

  let onContextMenuPopup = once(textboxContextMenu, "popupshowing");
  textboxContextMenu.openPopupAtScreen(0, 0, true);
  yield onContextMenuPopup;

  is(cmdUndo.getAttribute("disabled"), "true", "cmdUndo is disabled");
  is(cmdDelete.getAttribute("disabled"), "true", "cmdDelete is disabled");
  is(cmdSelectAll.getAttribute("disabled"), "true", "cmdSelectAll is disabled");

  // Cut/Copy/Paste items are enabled in context menu even if there
  // is no selection. See also Bug 1303033, and 1317322
  is(cmdCut.getAttribute("disabled"), "", "cmdCut is enabled");
  is(cmdCopy.getAttribute("disabled"), "", "cmdCopy is enabled");

  is(cmdPaste.getAttribute("disabled"), "", "cmdPaste is enabled");

  yield cleanup(toolbox);
});

function* cleanup(toolbox) {
  yield toolbox.destroy();
  gBrowser.removeCurrentTab();
}
