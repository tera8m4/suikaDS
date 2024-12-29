import { LitElement } from "lit";

export class Element extends LitElement {
  createRenderRoot() {
    return this;
  }
}
