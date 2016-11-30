package com.voicemynews.voicemynews.models;

/**
 * Provides a simple state object which describes various aspects of the side menu.
 */
public class SideMenuState {
    private boolean visible;
    private int selectedItem;

    public SideMenuState() {
        this.visible = false;
        this.selectedItem = -1;
    }

    public int getSelectedItem() {
        return selectedItem;
    }

    public void setSelectedItem(int selectedItem) {
        this.selectedItem = selectedItem;
    }

    public boolean isVisible() {
        return visible;
    }

    public void setVisible(boolean visible) {
        this.visible = visible;
    }
}
