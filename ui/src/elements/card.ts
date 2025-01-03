import { html } from "lit";
import { customElement, property, state } from "lit/decorators.js";
import { CardMessage } from "./message-controller";
import { Transport } from "../transport";
import { Element } from "./element";

@customElement("game-card")
export class Card extends Element {
  @property()
  message!: CardMessage;

  @state()
  isScreenshotLoading: boolean = false;

  onUpdateLastNote() {
    this.dispatchEvent(new CustomEvent("updateLastNote", { detail: { id: this.message.id } }));
  }

  async onUpdateWithNewScreenshot() {
    try {
      this.isScreenshotLoading = true;
      const image = await Transport.getInstance().requestNewScreenshot();
      this.message.image = image;
      this.isScreenshotLoading = false;
      this.onUpdateLastNote();
    } catch {

    }
  }

  render() {
    return html`<article class="card" aria-busy=${this.isScreenshotLoading}>
                       <div class="card-container"><img src="${this.message.image}" class="card-image"/></div>
                       <footer>
                       <h3>${this.message.message}</h3>
                       <ul>
                       <li><button @click="${this.onUpdateLastNote}">Update the last card with current image</button></li>
                       <li><button @click="${this.onUpdateWithNewScreenshot}">Update the last card with new image</button>
                       </footer>
                       </article>`;
  }
}
