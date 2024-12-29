import { html } from 'lit'
import { customElement } from 'lit/decorators.js'
import { MessageController } from './message-controller';
import { Element } from "./element";
import { AnkiTransport } from "./anki-transport";

@customElement("cards-list")
export class CardsList extends Element {


  private messagesController: MessageController = new MessageController(this);

  createRenderRoot() { return this; }

  onUpdateCard(cardID: number) {
    const card = this.messagesController.message.find(x => x.id == cardID);
    if (card?.image) {
      AnkiTransport.updateLastNote(card.image, "Picture");
    }
  }

  render() {
    return html`<div class="cards-list"><h2>Cards</h2>      ${this.messagesController.message.map(x => html`<article class="card">
                       <div class="card-container"><img src="${x.image}" class="card-image"/></div>
                       <footer>
                       <h3>${x.message}</h3>
                       <button @click="${() => this.onUpdateCard(x.id)}">Update last card</button>
                       </footer>
                       </article>`)}
    </div>`;
  }
}
